#[allow(non_snake_case)]
pub mod asset {
    #[allow(non_snake_case)]
    pub mod 测试中文 {
        #[allow(non_upper_case_globals)]
        pub const test_toml: &'static str = include_str!(concat!(
            env!("CARGO_MANIFEST_DIR"),
            "/asset/0测试空格 test space/0test.toml"
        ));
    }
    #[allow(non_upper_case_globals)]
    pub const example_image: &'static [u8] =
        include_bytes!(concat!(env!("CARGO_MANIFEST_DIR"), "/asset/example.png"));
    #[allow(non_upper_case_globals)]
    pub const test_glsl: &'static str =
        include_str!(concat!(env!("CARGO_MANIFEST_DIR"), "/asset/test.glsl"));
}
