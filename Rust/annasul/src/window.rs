#[cfg(test)]
mod tests {
    use winit::{
        application::ApplicationHandler,
        event::WindowEvent,
        event_loop::{ActiveEventLoop, ControlFlow, EventLoopBuilder},
        platform::windows::EventLoopBuilderExtWindows,
        window::{Window, WindowId},
    };

    struct App {
        window: Option<Window>,
    }

    impl App {
        fn new() -> Self {
            Self { window: None }
        }
    }

    impl ApplicationHandler for App {
        fn resumed(&mut self, event_loop: &ActiveEventLoop) {
            self.window = Some(
                event_loop
                    .create_window(Window::default_attributes())
                    .unwrap(),
            );
        }

        fn window_event(
            &mut self,
            event_loop: &ActiveEventLoop,
            _id: WindowId,
            event: WindowEvent,
        ) {
            match event {
                WindowEvent::CloseRequested => {
                    event_loop.exit();
                }
                WindowEvent::RedrawRequested => {
                    self.window.as_ref().unwrap().request_redraw();
                }
                WindowEvent::Touch(touch) => {
                    println!("Touch: {:?}", touch);
                }
                _ => {}
            }
        }
    }

    #[test]
    fn example() {
        let event_loop = EventLoopBuilder::default()
            .with_any_thread(true)
            .build()
            .unwrap();
        event_loop.set_control_flow(ControlFlow::Wait);

        event_loop.run_app(&mut App::new()).unwrap();
    }
}
