use std::sync::Arc;

use winit::{
    event::WindowEvent,
    event_loop::{EventLoop, EventLoopBuilder},
    window::Window,
};

#[derive(Debug)]
pub enum Error {
    // winit::error::EventLoopError 错误
    WinitEventLoopError(winit::error::EventLoopError),
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Error::WinitEventLoopError(e) => write!(f, "Event loop error: {}", e),
        }
    }
}

impl std::error::Error for Error {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            Error::WinitEventLoopError(e) => Some(e),
        }
    }
}

pub type Result<T> = std::result::Result<T, Error>;
pub struct Application {
    // 事件循环
    event_loop: EventLoop<()>,
}

#[derive(Debug, Default)]
struct App {
    window: Option<Arc<winit::window::Window>>,
}

impl winit::application::ApplicationHandler for App {
    fn resumed(&mut self, event_loop: &winit::event_loop::ActiveEventLoop) {
        self.window = Some(Arc::new(
            event_loop
                .create_window(Window::default_attributes())
                .unwrap(),
        ));
    }

    fn window_event(
        &mut self,
        event_loop: &winit::event_loop::ActiveEventLoop,
        window_id: winit::window::WindowId,
        event: winit::event::WindowEvent,
    ) {
        let _ = window_id;
        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }
            WindowEvent::RedrawRequested => {
                self.window.as_ref().unwrap().request_redraw();
            }
            _ => {}
        }
    }
}

impl Application {
    ///
    ///
    /// It can only be called in the main thread
    ///
    pub fn new() -> Result<Self> {
        let event_loop = EventLoop::new().map_err(Error::WinitEventLoopError)?;
        Ok(Self { event_loop })
    }
    ///
    ///
    /// Platfrom-specific: only available on Windows
    ///
    pub fn new_with_any_thread() -> Result<Self> {
        let mut event_loop = EventLoopBuilder::default();
        #[cfg(windows)]
        {
            use winit::platform::windows::EventLoopBuilderExtWindows;
            event_loop.with_any_thread(true);
        }
        #[cfg(not(windows))]
        {
            panic!("with_any_thread is only available on Windows");
        }
        let event_loop = event_loop.build().map_err(Error::WinitEventLoopError)?;
        Ok(Self { event_loop })
    }
    pub fn run(self) -> Result<()> {
        let mut app = App::default();
        self.event_loop
            .run_app(&mut app)
            .map_err(Error::WinitEventLoopError)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_application() {
        let app = Application::new_with_any_thread().unwrap();
        app.run().unwrap();
    }
}
