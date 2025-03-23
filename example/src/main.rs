//隐藏Windows上的控制台窗口
#![windows_subsystem = "windows"]

use annasul::include_asset;
use eframe::egui::{
    self, ColorImage, Image, TextureHandle, TextureOptions, UiBuilder, Vec2, ViewportCommand,
};
use wgpu::rwh::HasWindowHandle;

include_asset!();
fn main() -> Result<(), eframe::Error> {
    // 创建视口选项，设置视口的内部大小为320x240像素
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_min_inner_size([320.0, 240.0]),
        vsync: false,
        window_builder: Some(Box::new(|builder| {
            #[cfg(any(target_os = "windows", target_os = "macos"))]
            builder.with_transparent(true)
        })),
        ..Default::default()
    };

    // 运行egui应用程序
    eframe::run_native(
        "My egui App", // 应用程序的标题
        options,       // 视口选项
        Box::new(|cc| {
            // 为我们提供图像支持
            egui_extras::install_image_loaders(&cc.egui_ctx);
            // 创建并返回一个实现了eframe::App trait的对象
            Ok(Box::new(MyApp::new(cc)))
        }),
    )
}

//定义 MyApp 结构体
struct MyApp {
    image: TextureHandle,
    name: String,
    age: u32,
}

fn load_color_image_from_memory(image_data: &[u8]) -> Result<ColorImage, image::ImageError> {
    let image = image::load_from_memory(image_data)?;
    let size = [image.width() as _, image.height() as _];
    let image_buffer = image.to_rgba8();
    let pixels = image_buffer.as_flat_samples();
    Ok(ColorImage::from_rgba_unmultiplied(size, pixels.as_slice()))
}

fn load_image_from_memory(
    ctx: &egui::Context,
    name: &str,
    image_data: &[u8],
    options: TextureOptions,
) -> Result<TextureHandle, image::ImageError> {
    Ok(ctx.load_texture(name, load_color_image_from_memory(image_data)?, options))
}

//MyApp 结构体 new 函数
impl MyApp {
    fn new(cc: &eframe::CreationContext<'_>) -> Self {
        let window = cc.window_handle().unwrap();
        #[cfg(target_os = "windows")]
        window_vibrancy::apply_mica(window, None).unwrap();
        #[cfg(target_os = "macos")]
        window_vibrancy::apply_vibrancy(
            &window,
            window_vibrancy::NSVisualEffectMaterial::WindowBackground,
            None,
            None,
        )
        .unwrap();
        // 结构体赋初值
        Self {
            image: load_image_from_memory(
                &cc.egui_ctx,
                "example_image",
                asset::example_image,
                TextureOptions::LINEAR,
            )
            .unwrap(),
            name: "Arthur".to_owned(),
            age: 42,
        }
    }
}

//实现 eframe::App trait
impl eframe::App for MyApp {
    fn clear_color(&self, _visuals: &egui::Visuals) -> [f32; 4] {
        egui::Color32::TRANSPARENT.to_normalized_gamma_f32()
    }
    fn update(&mut self, ctx: &egui::Context, frame: &mut eframe::Frame) {
        ctx.set_visuals(egui::Visuals {
            window_fill: egui::Color32::MAGENTA,
            panel_fill: egui::Color32::TRANSPARENT,
            ..Default::default()
        });
        // 在中央面板上显示egui界面
        egui::CentralPanel::default().show(ctx, |ui| {
            // 显示标题
            ui.heading("My egui Application");
            // 创建一个水平布局
            ui.horizontal(|ui| {
                // 显示姓名标签
                let name_label = ui.label("Your name: ");
                // 显示姓名输入框（单行文本框）
                ui.text_edit_singleline(&mut self.name)
                    .labelled_by(name_label.id); // 关联标签
            });

            // 显示年龄滑块
            ui.add(egui::Slider::new(&mut self.age, 0..=120).text("age"));

            if ui.button("Increment").clicked() {
                // 点击按钮后将年龄加1
                self.age += 1;
            }

            // 显示问候语
            ui.label(format!("Hello '{}', age {}", self.name, self.age));
            // 显示图片
            ui.add(
                Image::new(&self.image)
                    .corner_radius(5)
                    .rotate((self.age as f32).to_radians(), Vec2::splat(0.5)),
            );
        });
    }
}
