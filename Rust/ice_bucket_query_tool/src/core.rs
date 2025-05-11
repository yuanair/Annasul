use eframe::egui::{self};

use crate::qq;

struct MyApp {
    qq_number: String,
    size: u16,
    error: Option<String>,
    image: Option<String>,
}

impl MyApp {
    fn new(cc: &eframe::CreationContext) -> Self {
        egui_extras::install_image_loaders(&cc.egui_ctx);
        Self {
            qq_number: "2283106661".to_string(),
            size: 0,
            error: None,
            image: qq::get_qq_avatar_url("2283106661", 0).ok(),
        }
    }
    fn set_error(&mut self, error: String) {
        self.error = Some(error);
    }
    fn clear_error(&mut self) {
        self.error = None;
    }
}

trait ResultExt<T, E: std::error::Error> {
    fn for_app(self, app: &mut MyApp) -> Self;
}

impl<T, E: std::error::Error> ResultExt<T, E> for Result<T, E> {
    fn for_app(self, app: &mut MyApp) -> Self {
        (&self).for_app(app);
        self
    }
}

impl<T, E: std::error::Error> ResultExt<T, E> for &Result<T, E> {
    fn for_app(self, app: &mut MyApp) -> Self {
        if let Err(e) = self {
            app.set_error(format!("{}", e));
            #[cfg(debug_assertions)]
            {
                panic!("ResultExt: {:?}, {}", e, e);
            }
        } else {
            app.clear_error();
        }
        self
    }
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        #[cfg(target_os = "android")]
        {
            egui::TopBottomPanel::top("title_bar").show(ctx, |ui| {
                ui.set_height(32.0);
            });
        }
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.horizontal(|ui| {
                let mut dirty = false;
                ui.label("QQ:");
                dirty |= ui.text_edit_singleline(&mut self.qq_number).changed();
                ui.label("Size:");
                dirty |= ui.add(egui::Slider::new(&mut self.size, 0..=5)).changed();
                if let Some(error) = &self.error {
                    ui.label(egui::RichText::new(error).color(egui::Color32::RED));
                }

                if dirty {
                    self.image = qq::get_qq_avatar_url(&self.qq_number, self.size)
                        .for_app(self)
                        .ok();
                }
            });
            if let Some(image) = &self.image {
                ui.add(egui::Image::new(image).max_width(200.0));
            }
        });
    }
}

#[cfg(target_os = "android")]
#[allow(dead_code)]
pub(crate) fn run(app: Option<winit::platform::android::activity::AndroidApp>) {
    eframe::run_native(
        "冰桶查询器",
        eframe::NativeOptions {
            android_app: app,
            ..Default::default()
        },
        Box::new(|cc| Ok(Box::new(MyApp::new(cc)))),
    )
    .unwrap();
}

#[cfg(not(target_os = "android"))]
#[allow(dead_code)]
pub(crate) fn run(app: Option<()>) {
    let _ = app;
    eframe::run_native(
        "冰桶查询器",
        eframe::NativeOptions {
            ..Default::default()
        },
        Box::new(|cc| Ok(Box::new(MyApp::new(cc)))),
    )
    .unwrap();
}
