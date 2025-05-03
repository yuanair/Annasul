//!
//! 引擎编辑器
//!

use eframe::egui;
use winit::{event::WindowEvent, event_loop::EventLoop, window::Window};

#[derive(Default)]
struct App {
    window: Option<Window>,
}

impl winit::application::ApplicationHandler for App {
    fn resumed(&mut self, event_loop: &winit::event_loop::ActiveEventLoop) {
        let window_attributes = winit::window::Window::default_attributes()
            .with_title("引擎编辑器")
            .with_inner_size(winit::dpi::LogicalSize::new(1280.0, 720.0));

        let window = event_loop.create_window(window_attributes).unwrap();
        self.window = Some(window);
    }

    fn window_event(
        &mut self,
        event_loop: &winit::event_loop::ActiveEventLoop,
        _window_id: winit::window::WindowId,
        event: WindowEvent,
    ) {
        match event {
            WindowEvent::CloseRequested => {
                event_loop.exit();
            }
            WindowEvent::RedrawRequested => {}
            _ => (),
        }
    }
}

struct MyApp {
    code: String,
    language: String,
}

impl MyApp {
    pub fn new(_ctx: &eframe::CreationContext) -> Self {
        Self {
            code: "println!(\"Hello, world!\");".to_string(),
            language: "rs".to_string(),
        }
    }
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, frame: &mut eframe::Frame) {
        #[cfg(target_os = "android")]
        {
            egui::TopBottomPanel::top("title_bar").show(ctx, |ui| {
                ui.set_height(30.0);
            });
        }
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.horizontal(|ui| {
                ui.label("Language:");
                ui.text_edit_singleline(&mut self.language);
            });
            let mut theme =
                egui_extras::syntax_highlighting::CodeTheme::from_memory(ui.ctx(), ui.style());
            ui.collapsing("Theme", |ui| {
                ui.group(|ui| {
                    theme.ui(ui);
                    theme.clone().store_in_memory(ui.ctx());
                });
            });
            let mut layouter = |ui: &egui::Ui, string: &str, wrap_width: f32| {
                let mut layout_job = egui_extras::syntax_highlighting::highlight(
                    ui.ctx(),
                    ui.style(),
                    &theme,
                    string,
                    &self.language,
                );
                layout_job.wrap.max_width = wrap_width;
                ui.fonts(|f| f.layout_job(layout_job))
            };
            egui::ScrollArea::vertical().show(ui, |ui| {
                ui.add(
                    egui::TextEdit::multiline(&mut self.code)
                        .font(egui::TextStyle::Monospace) // for cursor height
                        .code_editor()
                        .desired_rows(10)
                        .lock_focus(true)
                        .desired_width(f32::INFINITY)
                        .layouter(&mut layouter),
                );
            });
        });
    }
}

#[cfg(target_os = "android")]
pub fn test_winit(app: Option<winit::platform::android::activity::AndroidApp>) {
    eframe::run_native(
        "editor",
        eframe::NativeOptions {
            #[cfg(target_os = "android")]
            android_app: app,
            ..Default::default()
        },
        Box::new(|cc| Ok(Box::new(MyApp::new(cc)))),
    )
    .unwrap();
    // let event_loop = EventLoop::new().unwrap();
    // event_loop.run_app(&mut App::default()).unwrap();
}

#[cfg(not(target_os = "android"))]
pub fn test_winit(app: Option<()>) {
    eframe::run_native(
        "editor",
        eframe::NativeOptions {
            ..Default::default()
        },
        Box::new(|cc| Ok(Box::new(MyApp::new(cc)))),
    )
    .unwrap();
}
