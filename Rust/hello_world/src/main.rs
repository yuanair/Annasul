// Rust 2024
// rustc 1.86.0
// stable-x86_64-pc-windows-msvc

fn main() {
    let vec = vec![1, 2, 3, 4, 5];
    vec.iter().for_each(|&x| print!("{} ", x));
    println!();
}
