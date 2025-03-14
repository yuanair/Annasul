mod __asset;

fn main() {
    let config: toml::Value = toml::from_str(__asset::toml::test_toml).unwrap();
    println!("{:?}", config);
}
