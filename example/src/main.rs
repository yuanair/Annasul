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
        viewport: egui::ViewportBuilder::default().with_inner_size([320.0, 240.0]),
        vsync: false,
        window_builder: Some(Box::new(|builder| {
            #[cfg(any(target_os = "windows", target_os = "macos"))]
            builder.with_transparent(true).with_decorations(false)
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

            let app_rect = ui.max_rect();

            let title_bar_height = 32.0;
            let title_bar_rect = {
                let mut rect = app_rect;
                rect.max.y = rect.min.y + title_bar_height;
                rect
            };
            title_bar_ui(ui, title_bar_rect, "Title Bar");

            // Add the contents:
            let content_rect = {
                let mut rect = app_rect;
                rect.min.y = title_bar_rect.max.y;
                rect
            }
            .shrink(4.0);

            let response = ui.interact(
                app_rect,
                egui::Id::new("action_bar_interation"),
                egui::Sense::click_and_drag(),
            );

            // if response.dragged() {
            //     if response.is_pointer_button_down_on() {
            //         ui.ctx().send_viewport_cmd(egui::ViewportCommand::StartDrag);
            //     }
            // }

            // if response.double_clicked() {
            //     ui.ctx()
            //         .send_viewport_cmd(egui::ViewportCommand::Maximized(true));
            //     ui.ctx().request_repaint();
            // }
        });
    }
}

fn title_bar_ui(ui: &mut egui::Ui, title_bar_rect: eframe::epaint::Rect, title: &str) {
    use egui::{Align2, FontId, Id, PointerButton, Sense, UiBuilder, vec2};

    let painter = ui.painter();

    let title_bar_response = ui.interact(
        title_bar_rect,
        Id::new("title_bar"),
        Sense::click_and_drag(),
    );

    // Paint the title:
    painter.text(
        title_bar_rect.center(),
        Align2::CENTER_CENTER,
        title,
        FontId::proportional(20.0),
        ui.style().visuals.text_color(),
    );

    // Paint the line under the title:
    painter.line_segment(
        [
            title_bar_rect.left_bottom() + vec2(1.0, 0.0),
            title_bar_rect.right_bottom() + vec2(-1.0, 0.0),
        ],
        ui.visuals().widgets.noninteractive.bg_stroke,
    );

    // Interact with the title bar (drag to move window):
    if title_bar_response.double_clicked() {
        let is_maximized = ui.input(|i| i.viewport().maximized.unwrap_or(false));
        ui.ctx()
            .send_viewport_cmd(ViewportCommand::Maximized(!is_maximized));
    }

    if title_bar_response.drag_started_by(PointerButton::Primary) {
        ui.ctx().send_viewport_cmd(ViewportCommand::StartDrag);
    }

    ui.scope_builder(
        UiBuilder::new()
            .max_rect(title_bar_rect)
            .layout(egui::Layout::right_to_left(egui::Align::Center)),
        |ui| {
            ui.spacing_mut().item_spacing.x = 0.0;
            ui.visuals_mut().button_frame = false;
            ui.add_space(8.0);
            close_maximize_minimize(ui);
        },
    );
}

fn close_maximize_minimize(ui: &mut egui::Ui) {
    use egui::{Button, RichText};

    let button_height = 12.0;

    let close_response = ui
        .add(Button::new(RichText::new("❌").size(button_height)))
        .on_hover_text("Close the window");
    if close_response.clicked() {
        ui.ctx().send_viewport_cmd(egui::ViewportCommand::Close);
    }

    let is_maximized = ui.input(|i| i.viewport().maximized.unwrap_or(false));
    if is_maximized {
        let maximized_response = ui
            .add(Button::new(RichText::new("🗗").size(button_height)))
            .on_hover_text("Restore window");
        if maximized_response.clicked() {
            ui.ctx()
                .send_viewport_cmd(ViewportCommand::Maximized(false));
        }
    } else {
        let maximized_response = ui
            .add(Button::new(RichText::new("🗗").size(button_height)))
            .on_hover_text("Maximize window");
        if maximized_response.clicked() {
            ui.ctx().send_viewport_cmd(ViewportCommand::Maximized(true));
        }
    }

    let minimized_response = ui
        .add(Button::new(RichText::new("🗕").size(button_height)))
        .on_hover_text("Minimize the window");
    if minimized_response.clicked() {
        ui.ctx().send_viewport_cmd(ViewportCommand::Minimized(true));
    }
}
