//!
//! # Vulkan Renderer
//!
use std::sync::Arc;

use log::info;
use vulkano::{
    device::{physical::PhysicalDevice, Device, DeviceCreateInfo, DeviceExtensions, Queue, QueueCreateInfo},
    instance::{Instance, InstanceCreateInfo},
    library::VulkanLibrary, swapchain::Surface,
};

use super::{Error, Result};

pub struct SurfaceImpl {
    pub surface: Arc<Surface>,
}

impl super::Surface for SurfaceImpl {

}

pub struct Renderer {
    library: Arc<VulkanLibrary>,
    instance: Arc<Instance>,
    device: Arc<Device>,
    queue: Arc<Queue>,
}

impl Renderer {
    pub fn new() -> Result<Self> {
        let library = VulkanLibrary::new().map_err(|e| Error::APILoadingError(Box::new(e)))?;
        let instance = Instance::new(
            library.clone(),
            InstanceCreateInfo {
                ..Default::default()
            },
        )
        .map_err(|e| Error::APILoadingError(Box::new(e)))?;
        let physical_device = Self::choose_physical_device(&instance)?;
        let queue_create_infos = vec![
            QueueCreateInfo::default()
        ];
        let (device, mut queue_iter) = Device::new(physical_device, DeviceCreateInfo{
            enabled_extensions: Self::required_extensions(),
            queue_create_infos,
            ..Default::default()
        }).map_err(|e| Error::DeviceCreationError(Box::new(e)))?;
        let queue = queue_iter.next().unwrap();

        Ok(Renderer {
            library,
            instance,
            device,
            queue,
        })
    }

    ///
    /// # Choose a physical device
    ///
    fn choose_physical_device(
        instance: &Arc<Instance>,
    ) -> Result<Arc<PhysicalDevice>> {
        let mut physical_devices = instance
            .enumerate_physical_devices()
            .map_err(|e| Error::EnumeratePhysicalDevicesError(Box::new(e)))?
            .filter(|physical_device| {
                let is_ok = physical_device
                    .supported_extensions()
                    .contains(&Self::required_extensions());
                let properties = physical_device.properties();
                info!(
                    "Found {} physical device: {} (PCI Bus Info: Domain {:?}, Bus {:?}, Device {:?}, Function {:?})",
                    if is_ok { "supported" } else { "unsupported" },
                    properties.device_name,
                    properties.pci_domain,    
                    properties.pci_bus,
                    properties.pci_device,
                    properties.pci_function,
                );
                is_ok
            });
        loop {
            let physical_device = physical_devices
                .next()
                .ok_or(Error::NoSupportedPhysicalDeviceFound)?;
            break Ok(physical_device);
        }
    }

    ///
    /// # Required extensions
    ///
    fn required_extensions() -> DeviceExtensions {
        DeviceExtensions {
            khr_swapchain: true,
            ..Default::default()
        }
    }
}

impl super::Renderer for Renderer {
    fn create_surface(
        &self,
        window: &(impl raw_window_handle::HasWindowHandle + raw_window_handle::HasDisplayHandle),
    ) -> super::Result<Box<dyn super::Surface>>{
        unsafe{Surface::from_window_ref(self.instance.clone(), window)}.map(|surface| {Box::new(SurfaceImpl { surface }) as Box<dyn super::Surface>}).map_err(|e| Error::CreateSurfaceError(Box::new(e)))
    }
}
