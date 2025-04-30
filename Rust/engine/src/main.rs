use annasul_engine::{core, editor};

fn main() {
    println!("{}-{}", core::NAME, core::VERSION);
    editor::test_winit();
}
