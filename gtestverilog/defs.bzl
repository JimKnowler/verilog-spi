
def _gtest_verilog_testbench_impl(ctx):
    input = None
    verilated_header_dir = None

    for dep in ctx.attr.deps:
        headers = dep[CcInfo].compilation_context.direct_headers
        for header in headers:
            if not header.is_directory:
                fail("header is not a directory")
            
            input = header
            verilated_header_dir = header.path

    if None == verilated_header_dir:
        fail("missing header directory in output from verilator")
    
    ctx.actions.run(
        inputs = [input],
        outputs = [
            ctx.outputs.source_file,
            ctx.outputs.header_file
        ],
        arguments = [
            "--name", ctx.attr.name,
            "--verilated-header-dir", verilated_header_dir,
            "--output-source", ctx.outputs.source_file.path, 
            "--output-header", ctx.outputs.header_file.path
        ],
        executable = ctx.executable.generate_testbench,
        progress_message = "generating gtest-testbench for " + ctx.attr.name,
        mnemonic = "GTestVerilogTestbench"
    )

gtest_verilog_testbench = rule(
    implementation = _gtest_verilog_testbench_impl,
    attrs = {
        "deps" : attr.label_list(providers = [CcInfo]),
        "generate_testbench" : attr.label(executable=True, cfg="host", default="//gtestverilog:generate_testbench")
    },
    outputs = {
        "source_file": "%{name}.cpp",
        "header_file": "%{name}.h"
    },
)