use std::{io::Write, sync::Arc};

use annasul::{
    include_asset,
    render::vulkan::{CommandBufferBuilder, Instance, InstanceBuilder},
};
use winit::{
    application::ApplicationHandler,
    event::WindowEvent,
    event_loop::{ActiveEventLoop, ControlFlow, EventLoopBuilder},
    platform::windows::EventLoopBuilderExtWindows,
    window::{Window, WindowId},
};

struct App {
    window: Option<Arc<Window>>,
    instance: Option<Arc<Instance>>,
    render_info: Option<annasul::render::vulkan::RenderInfo>,
}

impl App {
    fn new() -> Self {
        Self {
            window: None,
            instance: None,
            render_info: None,
        }
    }
}

impl ApplicationHandler for App {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        self.window = Some(Arc::new(
            event_loop
                .create_window(Window::default_attributes())
                .unwrap(),
        ));
        let instance = InstanceBuilder::default()
            .with_mut_extensions(|ext| {
                *ext = ext.union(&InstanceBuilder::require_extension(event_loop).unwrap());
            })
            .build()
            .unwrap();
        self.instance = Some(instance.clone());
        self.render_info = Some(
            annasul::render::vulkan::Builder::new(instance.clone())
                .with_inner_size(self.window.as_ref().unwrap().inner_size().into())
                .with_surface(
                    annasul::render::vulkan::SurfaceBuilder::FromRawWindowHandle {
                        instance,
                        handle: self.window.as_ref().unwrap().clone(),
                    },
                )
                .build()
                .unwrap(),
        );
    }

    fn window_event(&mut self, event_loop: &ActiveEventLoop, _id: WindowId, event: WindowEvent) {
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

fn main() -> Result<(), Box<dyn std::error::Error>> {
    env_logger::Builder::from_default_env()
        .target(env_logger::Target::Stdout)
        .format(|buf, record| buf.write_fmt(format_args!("{}", record.args())))
        .init();
    let event_loop = EventLoopBuilder::default()
        .with_any_thread(true)
        .build()
        .unwrap();
    event_loop.set_control_flow(ControlFlow::Wait);

    event_loop.run_app(&mut App::new()).unwrap();
    Ok(())
}

// //隐藏Windows上的控制台窗口
// // #![windows_subsystem = "windows"]

// use std::{
//     io::Write,
//     sync::{Arc, RwLock, mpsc},
//     thread,
// };

// use annasul::include_asset;
// use eframe::egui::{
//     self, Color32, ColorImage, Image, RichText, TextureHandle, TextureOptions, Vec2,
// };
// use log::info;
// use wgpu::rwh::HasWindowHandle;

// enum Command {
//     RunRhai {
//         script: String,
//         result: Arc<RwLock<Option<Result<String, String>>>>,
//         output: Arc<RwLock<String>>,
//     },
//     Exit,
// }

// include_asset!();

// fn main() -> Result<(), eframe::Error> {
//     env_logger::Builder::from_default_env()
//         .target(env_logger::Target::Stdout)
//         .format(|buf, record| buf.write_fmt(format_args!("{}", record.args())))
//         .init();
//     // 创建通信通道
//     let (cmd_sender, cmd_receiver) = mpsc::channel::<Command>();

//     // 启动主线程处理 Rhai
//     let main_thread = thread::spawn(move || {
//         let mut app = MainThreadApp::new();
//         while let Ok(cmd) = cmd_receiver.recv() {
//             match cmd {
//                 Command::RunRhai {
//                     script,
//                     result,
//                     output,
//                 } => {
//                     app.rhai_engine.on_print(move |s| {
//                         *output.write().unwrap() = s.to_owned();
//                     });
//                     *result.write().unwrap() = Some(
//                         app.rhai_engine
//                             .eval::<rhai::Dynamic>(&script)
//                             .map(|v| format!("{v:?}"))
//                             .map_err(|e| format!("{e:?}")),
//                     );
//                 }
//                 Command::Exit => break,
//             }
//         }
//     });

//     // 启动 UI
//     let options = eframe::NativeOptions {
//         viewport: egui::ViewportBuilder::default().with_min_inner_size([320.0, 240.0]),
//         vsync: false,
//         window_builder: Some(Box::new(|builder| {
//             #[cfg(any(target_os = "windows", target_os = "macos"))]
//             builder.with_transparent(true)
//         })),
//         ..Default::default()
//     };

//     eframe::run_native(
//         "Rhai + egui Demo",
//         options,
//         Box::new(|cc| {
//             egui_extras::install_image_loaders(&cc.egui_ctx);
//             Ok(Box::new(UIThreadApp::new(cc, cmd_sender)))
//         }),
//     )?;
//     main_thread.join().unwrap();
//     Ok(())
// }

// struct MainThreadApp {
//     rhai_engine: rhai::Engine,
// }

// impl MainThreadApp {
//     fn new() -> Self {
//         let rhai_engine = rhai::Engine::new();
//         Self { rhai_engine }
//     }
// }

// struct UIThreadApp {
//     rhai_script: String,
//     rhai_output: Arc<RwLock<String>>,
//     rhai_result: Arc<RwLock<Option<Result<String, String>>>>,
//     main_thread_sender: std::sync::mpsc::Sender<Command>,
//     image: TextureHandle,
//     name: String,
//     age: u32,
// }

// fn load_color_image_from_memory(image_data: &[u8]) -> Result<ColorImage, image::ImageError> {
//     let image = image::load_from_memory(image_data)?;
//     let size = [image.width() as _, image.height() as _];
//     let image_buffer = image.to_rgba8();
//     let pixels = image_buffer.as_flat_samples();
//     Ok(ColorImage::from_rgba_unmultiplied(size, pixels.as_slice()))
// }

// fn load_image_from_memory(
//     ctx: &egui::Context,
//     name: &str,
//     image_data: &[u8],
//     options: TextureOptions,
// ) -> Result<TextureHandle, image::ImageError> {
//     Ok(ctx.load_texture(name, load_color_image_from_memory(image_data)?, options))
// }

// //MyApp 结构体 new 函数
// impl UIThreadApp {
//     fn new(cc: &eframe::CreationContext<'_>, sender: std::sync::mpsc::Sender<Command>) -> Self {
//         let window = cc.window_handle().unwrap();
//         #[cfg(target_os = "windows")]
//         window_vibrancy::apply_mica(window, None).unwrap();
//         #[cfg(target_os = "macos")]
//         window_vibrancy::apply_vibrancy(
//             &window,
//             window_vibrancy::NSVisualEffectMaterial::WindowBackground,
//             None,
//             None,
//         )
//         .unwrap();
//         Self {
//             rhai_script: "0".to_string(),
//             main_thread_sender: sender,
//             rhai_output: Arc::new(RwLock::new("".to_string())),
//             rhai_result: Arc::new(RwLock::new(None)),
//             image: load_image_from_memory(
//                 &cc.egui_ctx,
//                 "example_image",
//                 asset::example_image,
//                 TextureOptions::LINEAR,
//             )
//             .unwrap(),
//             name: "Arthur".to_owned(),
//             age: 42,
//         }
//     }
// }

// //实现 eframe::App trait
// impl eframe::App for UIThreadApp {
//     fn clear_color(&self, _visuals: &egui::Visuals) -> [f32; 4] {
//         egui::Color32::TRANSPARENT.to_normalized_gamma_f32()
//     }
//     fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
//         ctx.set_visuals(egui::Visuals {
//             window_fill: egui::Color32::MAGENTA,
//             panel_fill: egui::Color32::TRANSPARENT,
//             ..Default::default()
//         });
//         // 在中央面板上显示egui界面
//         egui::CentralPanel::default().show(ctx, |ui| {
//             // 显示标题
//             ui.heading("My egui Application");
//             // 创建一个水平布局
//             ui.horizontal(|ui| {
//                 // 显示姓名标签
//                 let name_label = ui.label("Your name: ");
//                 // 显示姓名输入框（单行文本框）
//                 ui.text_edit_singleline(&mut self.name)
//                     .labelled_by(name_label.id); // 关联标签
//             });

//             // 显示年龄滑块
//             ui.add(egui::Slider::new(&mut self.age, 0..=120).text("age"));

//             if ui.button("Increment").clicked() {
//                 info!("age += 1");
//                 self.age += 1;
//             }

//             // 显示问候语
//             ui.label(format!("Hello '{}', age {}", self.name, self.age));
//             // 运行Rhai脚本
//             let mut is_running = ui.text_edit_singleline(&mut self.rhai_script).lost_focus();
//             if let Some(result) = &*self.rhai_result.read().unwrap() {
//                 ui.label(match result {
//                     Ok(v) => RichText::new(v).color(Color32::GREEN),
//                     Err(e) => RichText::new(e).color(Color32::RED),
//                 });
//             }
//             is_running |= ui.button("Run").clicked();
//             ui.label(self.rhai_output.read().unwrap().as_str());
//             if is_running {
//                 self.main_thread_sender
//                     .send(Command::RunRhai {
//                         script: self.rhai_script.clone(),
//                         result: self.rhai_result.clone(),
//                         output: self.rhai_output.clone(),
//                     })
//                     .unwrap();
//             }

//             ui.add(
//                 Image::new(&self.image)
//                     .corner_radius(5)
//                     .rotate((self.age as f32).to_radians(), Vec2::splat(0.5)),
//             );
//         });
//     }
//     fn on_exit(&mut self, _gl: Option<&eframe::glow::Context>) {
//         self.main_thread_sender.send(Command::Exit).unwrap();
//     }
// }
