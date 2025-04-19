use std::{
    any::Any,
    borrow::Cow,
    fmt::{Display, Formatter},
    sync::Arc,
};

use raw_window_handle::{HandleError, HasDisplayHandle, HasWindowHandle};
use vulkano::{
    LoadingError, Validated, ValidationError, VulkanError, VulkanLibrary,
    command_buffer::{
        AutoCommandBufferBuilder, CommandBufferUsage, PrimaryAutoCommandBuffer,
        RenderPassBeginInfo, SubpassBeginInfo, SubpassEndInfo,
        allocator::{StandardCommandBufferAllocator, StandardCommandBufferAllocatorCreateInfo},
    },
    device::{
        Device, DeviceCreateInfo, DeviceExtensions, Queue, QueueCreateInfo,
        physical::PhysicalDevice,
    },
    image::{Image, ImageUsage, view::ImageView},
    instance::{InstanceCreateInfo, InstanceExtensions},
    render_pass::{Framebuffer, FramebufferCreateInfo, RenderPass},
    swapchain::SwapchainCreateInfo,
};

pub use vulkano::instance::Instance;

///
/// Error
///
#[derive(Debug)]
pub enum Error {
    /// Failed to load vulkan library
    LoadLibraryError(LoadingError),
    /// Failed to create vulkan instance
    CreateInstanceError(Validated<VulkanError>),
    /// Failed to create surface
    CreateSurfaceError(vulkano::swapchain::FromWindowError),
    /// Failed to enumerate physical devices
    EnumeratePhysicalDevicesError(VulkanError),
    /// Failed to choose physical device
    ChoosePhysicalDeviceError(Cow<'static, str>),
    /// Failed to create device
    CreateDeviceError(Validated<VulkanError>),
    /// Failed to create swapchain
    CreateSwapchainError(Validated<VulkanError>),
    /// Failed to create command buffer
    CreateCommandBufferError(Validated<VulkanError>),
    /// Failed to begin render pass
    BeginRenderPassError(Box<ValidationError>),
    /// Failed to end render pass
    EndRenderPassError(Box<ValidationError>),
    /// Failed to require extensions
    RequiredExtensionsError(HandleError),
    /// Failed to create render pass
    CreateRenderPassError(Validated<VulkanError>),
}

impl Display for Error {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        match self {
            Error::LoadLibraryError(e) => write!(f, "Failed to load vulkan library: {}", e),
            Error::CreateInstanceError(e) => write!(f, "Failed to create vulkan instance: {}", e),
            Error::CreateSurfaceError(e) => write!(f, "Failed to create surface: {}", e),
            Error::EnumeratePhysicalDevicesError(e) => {
                write!(f, "Failed to enumerate physical devices: {}", e)
            }
            Error::ChoosePhysicalDeviceError(e) => {
                write!(f, "Failed to choose physical device: {}", e)
            }
            Error::CreateDeviceError(e) => write!(f, "Failed to create device: {}", e),
            Error::CreateSwapchainError(e) => write!(f, "Failed to create swapchain: {}", e),
            Error::CreateCommandBufferError(e) => {
                write!(f, "Failed to create command buffer: {}", e)
            }
            Error::BeginRenderPassError(e) => write!(f, "Failed to begin render pass: {}", e),
            Error::EndRenderPassError(e) => write!(f, "Failed to end render pass: {}", e),
            Error::RequiredExtensionsError(e) => {
                write!(f, "Failed to require extensions: {}", e)
            }
            Error::CreateRenderPassError(e) => {
                write!(f, "Failed to create render pass: {}", e)
            }
        }
    }
}

impl std::error::Error for Error {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            Error::LoadLibraryError(e) => Some(e),
            Error::CreateInstanceError(e) => Some(e),
            Error::CreateSurfaceError(e) => Some(e),
            Error::EnumeratePhysicalDevicesError(e) => Some(e),
            Error::ChoosePhysicalDeviceError(_) => None,
            Error::CreateDeviceError(e) => Some(e),
            Error::CreateSwapchainError(e) => Some(e),
            Error::CreateCommandBufferError(e) => Some(e),
            Error::BeginRenderPassError(e) => Some(e),
            Error::EndRenderPassError(e) => Some(e),
            Error::RequiredExtensionsError(e) => Some(e),
            Error::CreateRenderPassError(e) => Some(e),
        }
    }
}

///
/// Result
///
pub type Result<T> = std::result::Result<T, Error>;

///
/// Render Info
///
pub struct RenderInfo {
    pub device: Arc<Device>,
    pub queues: Vec<Arc<Queue>>,
    pub surface: Option<(
        Arc<vulkano::swapchain::Surface>,
        Arc<vulkano::swapchain::Swapchain>,
        Vec<Arc<vulkano::image::Image>>,
        Vec<Arc<Framebuffer>>,
    )>,
}

///
/// Builder
///
pub struct Builder<T>
where
    T: HasWindowHandle + HasDisplayHandle + Any + Send + Sync,
{
    instance: Arc<Instance>,
    surface_builder: Option<SurfaceBuilder<T>>,
    inner_size: Option<[u32; 2]>,
    choose_physics_devices: Option<
        Box<
            dyn Fn(
                &Instance,
                &dyn ExactSizeIterator<Item = Arc<PhysicalDevice>>,
            ) -> Result<Arc<PhysicalDevice>>,
        >,
    >,
}
impl<T> Builder<T>
where
    T: HasWindowHandle + HasDisplayHandle + Any + Send + Sync,
{
    pub fn new(instance: Arc<Instance>) -> Self {
        Self {
            instance,
            surface_builder: None,
            inner_size: None,
            choose_physics_devices: None,
        }
    }
    pub fn with_surface(mut self, surface_builder: SurfaceBuilder<T>) -> Self
    where
        T: HasWindowHandle + HasDisplayHandle + Any + Send + Sync,
    {
        self.surface_builder = Some(surface_builder);
        self
    }

    pub fn with_inner_size(mut self, size: [u32; 2]) -> Self {
        self.inner_size = Some(size);
        self
    }

    pub fn with_choose_physics_devices<F>(mut self, choose_physics_devices: F) -> Self
    where
        F: Fn(
                &Instance,
                &dyn ExactSizeIterator<Item = Arc<PhysicalDevice>>,
            ) -> Result<Arc<PhysicalDevice>>
            + 'static,
    {
        self.choose_physics_devices = Some(Box::new(choose_physics_devices));
        self
    }

    pub fn build(self) -> Result<RenderInfo> {
        let physical_device = match self.choose_physics_devices {
            Some(choose_physics_devices) => {
                let physical_devices = self
                    .instance
                    .enumerate_physical_devices()
                    .map_err(Error::EnumeratePhysicalDevicesError)?;
                choose_physics_devices(&self.instance, &physical_devices)?
            }
            None => {
                let mut physical_devices = self
                    .instance
                    .enumerate_physical_devices()
                    .map_err(Error::EnumeratePhysicalDevicesError)?;
                physical_devices
                    .next()
                    .ok_or(Error::ChoosePhysicalDeviceError(
                        "No physical device found".into(),
                    ))?
            }
        };
        let enabled_extensions = DeviceExtensions {
            khr_swapchain: true,
            ..Default::default()
        };
        let (device, queues) = Device::new(
            physical_device,
            DeviceCreateInfo {
                enabled_extensions,
                queue_create_infos: vec![QueueCreateInfo::default()],
                ..Default::default()
            },
        )
        .map_err(Error::CreateDeviceError)?;
        let queues = queues.collect();

        match self.surface_builder {
            Some(surface_builder) => {
                let surface = surface_builder.build()?;
                let surface_capabilities = device
                    .physical_device()
                    .surface_capabilities(&surface, Default::default())
                    .unwrap();
                let (image_format, _) = device
                    .physical_device()
                    .surface_formats(&surface, Default::default())
                    .unwrap()[0];
                let (swapchain, images) = vulkano::swapchain::Swapchain::new(
                    device.clone(),
                    surface.clone(),
                    SwapchainCreateInfo {
                        min_image_count: surface_capabilities.min_image_count.max(2),
                        image_usage: ImageUsage::COLOR_ATTACHMENT,
                        image_color_space: vulkano::swapchain::ColorSpace::SrgbNonLinear,
                        image_format,
                        image_extent: self.inner_size.unwrap(),
                        composite_alpha: surface_capabilities
                            .supported_composite_alpha
                            .into_iter()
                            .next()
                            .unwrap(),
                        ..Default::default()
                    },
                )
                .map_err(|e| Error::CreateSwapchainError(e))?;
                let render_pass = vulkano::single_pass_renderpass!(
                    device.clone(),
                    attachments: {
                        // `color` is a custom name we give to the first and only attachment.
                        color: {
                            // `format: <ty>` indicates the type of the format of the image. This has to be
                            // one of the types of the `vulkano::format` module (or alternatively one of
                            // your structs that implements the `FormatDesc` trait). Here we use the same
                            // format as the swapchain.
                            format: swapchain.image_format(),
                            // `samples: 1` means that we ask the GPU to use one sample to determine the
                            // value of each pixel in the color attachment. We could use a larger value
                            // (multisampling) for antialiasing. An example of this can be found in
                            // msaa-renderpass.rs.
                            samples: 1,
                            // `load_op: Clear` means that we ask the GPU to clear the content of this
                            // attachment at the start of the drawing.
                            load_op: Clear,
                            // `store_op: Store` means that we ask the GPU to store the output of the draw
                            // in the actual image. We could also ask it to discard the result.
                            store_op: Store,
                        },
                    },
                    pass: {
                        // We use the attachment named `color` as the one and only color attachment.
                        color: [color],
                        // No depth-stencil attachment is indicated with empty brackets.
                        depth_stencil: {},
                    },
                )
                .map_err(|e| Error::CreateRenderPassError(e))?;

                let framebuffers = window_size_dependent_setup(&images, &render_pass);
                Ok(RenderInfo {
                    device,
                    queues,
                    surface: Some((surface, swapchain, images, framebuffers)),
                })
            }
            None => Ok(RenderInfo {
                device,
                queues,
                surface: None,
            }),
        }
    }
}

///
/// Instance Builder
///
#[derive(Debug, Clone, Default)]
pub struct InstanceBuilder {
    extensions: InstanceExtensions,
}

///
/// Surface Builder
///
#[derive(Debug, Clone)]
pub enum SurfaceBuilder<T>
where
    T: HasWindowHandle + HasDisplayHandle + Any + Send + Sync,
{
    /// Create surface from raw-window-handle
    FromRawWindowHandle {
        instance: Arc<Instance>,
        handle: Arc<T>,
    },
}

///
/// Command Buffer Builder
///
#[derive(Debug, Clone)]
pub struct CommandBufferBuilder<F>
where
    F: Fn(&mut AutoCommandBufferBuilder<PrimaryAutoCommandBuffer>) -> Result<()>,
{
    queue: Arc<Queue>,
    pass_begin_info: RenderPassBeginInfo,
    subpass_begin_info: SubpassBeginInfo,
    subpass_end_info: SubpassEndInfo,
    render_pass: Option<F>,
}

impl<F> CommandBufferBuilder<F>
where
    F: Fn(&mut AutoCommandBufferBuilder<PrimaryAutoCommandBuffer>) -> Result<()>,
{
    pub fn new(queue: Arc<Queue>, framebuffer: Arc<Framebuffer>) -> Self {
        Self {
            queue,
            pass_begin_info: RenderPassBeginInfo::framebuffer(framebuffer),
            subpass_begin_info: SubpassBeginInfo::default(),
            subpass_end_info: SubpassEndInfo::default(),
            render_pass: None,
        }
    }

    pub fn with_subpass_begin_info(mut self, subpass: SubpassBeginInfo) -> Self {
        self.subpass_begin_info = subpass;
        self
    }

    pub fn with_subpass_end_info(mut self, subpass: SubpassEndInfo) -> Self {
        self.subpass_end_info = subpass;
        self
    }

    pub fn with_render_pass(mut self, render_pass: F) -> Self {
        self.render_pass = Some(render_pass);
        self
    }

    pub fn build(self) -> Result<Arc<PrimaryAutoCommandBuffer>> {
        let mut cb = AutoCommandBufferBuilder::primary(
            Arc::new(StandardCommandBufferAllocator::new(
                self.queue.device().clone(),
                StandardCommandBufferAllocatorCreateInfo::default(),
            )),
            self.queue.queue_family_index(),
            CommandBufferUsage::OneTimeSubmit,
        )
        .map_err(Error::CreateCommandBufferError)?;
        cb.begin_render_pass(self.pass_begin_info, self.subpass_begin_info)
            .map_err(Error::BeginRenderPassError)?;
        if let Some(render_pass) = self.render_pass {
            render_pass(&mut cb)?;
        }
        cb.end_render_pass(self.subpass_end_info)
            .map_err(Error::EndRenderPassError)?;
        let cb = cb.build().map_err(Error::CreateCommandBufferError)?;
        Ok(cb)
    }
}

impl InstanceBuilder {
    pub fn new() -> Self {
        Self {
            extensions: Self::default_extensions(),
        }
    }

    pub fn default_extensions() -> InstanceExtensions {
        InstanceExtensions {
            khr_surface: true,
            ..Default::default()
        }
    }

    pub fn require_extension(event_loop: &impl HasDisplayHandle) -> Result<InstanceExtensions> {
        vulkano::swapchain::Surface::required_extensions(event_loop)
            .map_err(Error::RequiredExtensionsError)
    }

    ///
    /// extensions
    ///
    pub fn extensions(&self) -> &InstanceExtensions {
        &self.extensions
    }

    pub fn with_mut_extensions<F>(mut self, f: F) -> Self
    where
        F: FnOnce(&mut InstanceExtensions),
    {
        f(&mut self.extensions);
        self
    }

    pub fn build(self) -> Result<Arc<Instance>> {
        let lib = VulkanLibrary::new().map_err(Error::LoadLibraryError)?;
        let instance = Instance::new(
            lib,
            InstanceCreateInfo {
                enabled_extensions: self.extensions,
                ..Default::default()
            },
        )
        .map_err(Error::CreateInstanceError)?;
        Ok(instance)
    }
}

impl<T> SurfaceBuilder<T>
where
    T: HasWindowHandle + HasDisplayHandle + Any + Send + Sync,
{
    pub fn build(self) -> Result<Arc<vulkano::swapchain::Surface>> {
        match self {
            SurfaceBuilder::FromRawWindowHandle { instance, handle } => {
                let surface = vulkano::swapchain::Surface::from_window(instance.clone(), handle)
                    .map_err(Error::CreateSurfaceError)?;
                Ok(surface)
            }
        }
    }
}

fn window_size_dependent_setup(
    images: &[Arc<Image>],
    render_pass: &Arc<RenderPass>,
) -> Vec<Arc<Framebuffer>> {
    images
        .iter()
        .map(|image| {
            let view = ImageView::new_default(image.clone()).unwrap();

            Framebuffer::new(
                render_pass.clone(),
                FramebufferCreateInfo {
                    attachments: vec![view],
                    ..Default::default()
                },
            )
            .unwrap()
        })
        .collect::<Vec<_>>()
}
