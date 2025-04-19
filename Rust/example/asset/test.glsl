#version 450    
struct VertexInput {
    layout(location = 0) in vec2 position,
    [[location(1)]] tex_coord: vec2<f32>,
};

struct FragmentOutput {
    [[location(0)]] color: vec4<f32>,
};

[[group(0), binding(0)]]
var texture: texture_2d<f32>;

[[group(0), binding(1)]]
var sampler: sampler;

[[stage(vertex)]]
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    out.position = vec2<f32>(in.position.x, -in.position.y);
    out.tex_coord = in.tex_coord;
    return out;
}

[[stage(fragment)]]
fn fs_main(in: VertexOutput) -> FragmentOutput {
    var out: FragmentOutput;
    out.color = texture.sample(sampler, in.tex_coord);
    return out;
}