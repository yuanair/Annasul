use annasul::include_asset;

use annasul::render::RenderDevice;
use winit::application::ApplicationHandler;
use winit::event::{ElementState, KeyEvent, WindowEvent};
use winit::event_loop::{ActiveEventLoop, ControlFlow, EventLoop};
use winit::keyboard::{Key, NamedKey};
use winit::platform::windows::WindowAttributesExtWindows;
use winit::window::{Window, WindowId};

include_asset!();

struct App {
    device: RenderDevice,
}

impl App {
    fn new<T>(
        event_loop: &winit::event_loop::EventLoop<T>,
    ) -> Result<Self, annasul::render::Error> {
        let device = RenderDevice::new(event_loop)?;
        Ok(Self { device })
    }
}

impl ApplicationHandler for App {
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

        let rcx = self.device.create_rcx(window);

        self.device.add_rcx(window_id, rcx);
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, id: WindowId, event: WindowEvent) {
        match event {
            WindowEvent::CloseRequested => {
                self.device.remove_rcx(id).unwrap();
                if self.device.windows_iter().count() == 0 {
                    event_loop.exit();
                }
            }
            WindowEvent::Resized(_) => {
                self.device.resize_rcx(id).unwrap();
            }
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
                let rcx = self.device.create_rcx(window);

                self.device.add_rcx(window_id, rcx);
            }
            WindowEvent::RedrawRequested => self.device.render_rcx(id).unwrap(),
            _ => (),
        }
    }
    fn about_to_wait(&mut self, _event_loop: &ActiveEventLoop) {
        self.device.all_window_request_redraw();
    }
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

    let mut app = App::new(&event_loop)?;
    event_loop.run_app(&mut app)?;

    Ok(())
}
