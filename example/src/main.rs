use annasul::include_asset;

include_asset!();

fn main() {
    let config: toml::Value = toml::from_str(asset::测试中文::test_toml).unwrap();
    println!("{:?}", config);
}
