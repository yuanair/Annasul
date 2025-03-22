use std::sync::Arc;

use annasul::include_asset;

use annasul::render::{Renderer, RendererBuilder};
use wgpu::naga::ShaderStage;
use wgpu::naga::back::glsl;
use wgpu::util::{BufferInitDescriptor, DeviceExt};
use wgpu::{
    Adapter, BindGroup, BindGroupLayout, Buffer, Device, Instance, Queue, RenderPipeline, Sampler,
    Surface, Texture, TextureView, VertexState,
};
use winit::application::ApplicationHandler;
use winit::event::{ElementState, KeyEvent, WindowEvent};
use winit::event_loop::{ActiveEventLoop, ControlFlow, EventLoop};
use winit::keyboard::{Key, NamedKey};
use winit::platform::windows::WindowAttributesExtWindows;
use winit::window::{Window, WindowId};

include_asset!();

struct App<'a> {
    window: Arc<Window>,
    surface: Surface<'a>,
    instance: Instance,
    device: Device,
    adapter: Adapter,
    queue: Queue,
    render_pipeline: RenderPipeline,
    vertex_buffer: Buffer,
    index_buffer: Buffer,
    bind_group: BindGroup,
    bind_group_layout: BindGroupLayout,
    texture: Texture,
    texture_view: TextureView,
    sampler: Sampler,
}

impl<'a> App<'a> {
    async fn new<T>(event_loop: &winit::event_loop::EventLoop<T>) -> annasul::render::Result<Self> {
        let window = Arc::new(
            event_loop
                .create_window(
                    Window::default_attributes()
                        .with_no_redirection_bitmap(true)
                        .with_decorations(true)
                        .with_transparent(true)
                        .with_blur(true)
                        .with_visible(false),
                )
                .unwrap(),
        );
        window_vibrancy::apply_mica(&window, None).unwrap();
        let instance = Instance::default();
        let surface = instance.create_surface(window.clone()).unwrap();
        let adapter = instance
            .request_adapter(&wgpu::RequestAdapterOptions::default())
            .await
            .unwrap();
        let (device, queue) = adapter
            .request_device(&wgpu::DeviceDescriptor::default(), None)
            .await
            .unwrap();
        let (texture, texture_view, sampler) = Self::create_texture(&device);
        let (render_pipeline, bind_group, bind_group_layout) =
            Self::create_render_pipeline(&device, &texture_view, &sampler);

        // 创建顶点和索引缓冲区
        let vertex_buffer = Self::create_vertex_buffer(&device);
        let index_buffer = Self::create_index_buffer(&device);

        Ok(Self {
            window,
            surface,
            instance,
            device,
            adapter,
            queue,
            render_pipeline,
            vertex_buffer,
            index_buffer,
            bind_group,
            bind_group_layout,
            texture,
            texture_view,
            sampler,
        })
    }
    fn create_render_pipeline(
        device: &Device,
        texture_view: &TextureView,
        sampler: &Sampler,
    ) -> (RenderPipeline, BindGroup, BindGroupLayout) {
        let shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: None,
            source: wgpu::ShaderSource::Glsl {
                shader: asset::test_glsl.into(),
                defines: vec![].into_iter().collect(),
                stage: ShaderStage::Fragment,
            },
        });
        let (bind_group, bind_group_layout) =
            Self::create_bind_group(device, texture_view, sampler);
        let pipeline_layout = device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
            label: None,
            bind_group_layouts: &[&bind_group_layout],
            push_constant_ranges: &[],
        });
        let render_pipeline = device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
            label: None,
            layout: Some(&pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: Some("vs_main"),
                buffers: &[],
                compilation_options: Default::default(),
            },
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList,
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw,
                cull_mode: None,
                polygon_mode: wgpu::PolygonMode::Fill,
                conservative: false,
                ..Default::default()
            },
            depth_stencil: None,
            multisample: wgpu::MultisampleState {
                count: 1,
                mask: !0,
                alpha_to_coverage_enabled: false,
            },
            fragment: Some(wgpu::FragmentState {
                module: &shader,
                entry_point: Some("fs_main"),
                targets: &[Some(wgpu::ColorTargetState {
                    format: wgpu::TextureFormat::Bgra8UnormSrgb,
                    blend: Some(wgpu::BlendState {
                        color: wgpu::BlendComponent {
                            src_factor: wgpu::BlendFactor::SrcAlpha,
                            dst_factor: wgpu::BlendFactor::OneMinusSrcAlpha,
                            operation: wgpu::BlendOperation::Add,
                        },
                        alpha: wgpu::BlendComponent {
                            src_factor: wgpu::BlendFactor::One,
                            dst_factor: wgpu::BlendFactor::One,
                            operation: wgpu::BlendOperation::Add,
                        },
                    }),
                    write_mask: wgpu::ColorWrites::ALL,
                })],
                compilation_options: Default::default(),
            }),
            cache: None,
            multiview: None,
        });
        (render_pipeline, bind_group, bind_group_layout)
    }

    fn create_vertex_buffer(device: &Device) -> Buffer {
        device.create_buffer_init(&BufferInitDescriptor {
            label: None,
            contents: &[],
            usage: wgpu::BufferUsages::VERTEX,
        })
    }

    fn create_index_buffer(device: &Device) -> Buffer {
        device.create_buffer_init(&BufferInitDescriptor {
            label: None,
            contents: &[],
            usage: wgpu::BufferUsages::INDEX,
        })
    }

    fn create_texture(device: &Device) -> (Texture, TextureView, Sampler) {
        let size = wgpu::Extent3d {
            width: 1,
            height: 1,
            depth_or_array_layers: 1,
        };
        let texture = device.create_texture(&wgpu::TextureDescriptor {
            label: None,
            size,
            mip_level_count: 1,
            sample_count: 1,
            dimension: wgpu::TextureDimension::D2,
            format: wgpu::TextureFormat::Rgba8Unorm,
            usage: wgpu::TextureUsages::TEXTURE_BINDING | wgpu::TextureUsages::COPY_DST,
            view_formats: &[wgpu::TextureFormat::Rgba8Unorm],
        });
        let texture_view = texture.create_view(&wgpu::TextureViewDescriptor::default());
        let sampler = device.create_sampler(&wgpu::SamplerDescriptor {
            label: None,
            address_mode_u: wgpu::AddressMode::ClampToEdge,
            address_mode_v: wgpu::AddressMode::ClampToEdge,
            address_mode_w: wgpu::AddressMode::ClampToEdge,
            mag_filter: wgpu::FilterMode::Nearest,
            min_filter: wgpu::FilterMode::Nearest,
            mipmap_filter: wgpu::FilterMode::Nearest,
            lod_min_clamp: 0.0,
            lod_max_clamp: 100.0,
            ..Default::default()
        });
        (texture, texture_view, sampler)
    }

    fn create_bind_group(
        device: &Device,
        texture_view: &TextureView,
        sampler: &Sampler,
    ) -> (BindGroup, BindGroupLayout) {
        let bind_group_layout = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
            entries: &[
                wgpu::BindGroupLayoutEntry {
                    binding: 0,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Texture {
                        sample_type: wgpu::TextureSampleType::Float { filterable: true },
                        view_dimension: wgpu::TextureViewDimension::D2,
                        multisampled: false,
                    },
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    binding: 1,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Sampler(wgpu::SamplerBindingType::Filtering),
                    count: None,
                },
            ],
            label: None,
        });

        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: wgpu::BindingResource::TextureView(texture_view),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: wgpu::BindingResource::Sampler(sampler),
                },
            ],
            label: None,
        });

        (bind_group, bind_group_layout)
    }
}

impl<'a> ApplicationHandler for App<'a> {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        let window = event_loop
            .create_window(
                Window::default_attributes()
                    .with_no_redirection_bitmap(true)
                    .with_decorations(true)
                    .with_transparent(true),
            )
            .unwrap();

        // window_vibrancy::apply_mica(&window, None).unwrap();
        let window_id = window.id();
        self.window.set_visible(true);
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, id: WindowId, event: WindowEvent) {
        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }
            WindowEvent::Resized(_) => {}
            WindowEvent::KeyboardInput {
                event:
                    KeyEvent {
                        logical_key: Key::Named(NamedKey::Space),
                        state: ElementState::Pressed,
                        ..
                    },
                ..
            } => {
                let window = event_loop
                    .create_window(
                        Window::default_attributes()
                            .with_transparent(true)
                            .with_blur(true),
                    )
                    .unwrap();
                window_vibrancy::apply_mica(&window, None).unwrap();
                let window_id = window.id();
            }
            WindowEvent::RedrawRequested => {}
            _ => (),
        }
    }
    fn about_to_wait(&mut self, _event_loop: &ActiveEventLoop) {}
}

// Intentionally use `fn main` for clarity
fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut builder = env_logger::Builder::from_default_env();
    #[cfg(debug_assertions)]
    builder.target(env_logger::Target::Stdout);
    #[cfg(not(debug_assertions))]
    builder.target(env_logger::Target::Pipe(Box::new(std::fs::File::open(
        "log.log",
    )?)));
    builder.init();
    log::info!(
        "platform: {}_{}",
        std::env::consts::OS,
        std::env::consts::ARCH
    );
    let event_loop = EventLoop::new()?;

    // ControlFlow::Poll continuously runs the event loop, even if the OS hasn't
    // dispatched any events. This is ideal for games and similar applications.
    // event_loop.set_control_flow(ControlFlow::Poll);

    // ControlFlow::Wait pauses the event loop if no events are available to process.
    // This is ideal for non-game applications that only update in response to user
    // input, and uses significantly less power/CPU time than ControlFlow::Poll.
    event_loop.set_control_flow(ControlFlow::Wait);

    let mut app = pollster::block_on(App::new(&event_loop))?;
    event_loop.run_app(&mut app)?;

    Ok(())
}
