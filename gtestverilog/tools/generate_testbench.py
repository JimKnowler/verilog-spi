import argparse
import os
import re

parser = argparse.ArgumentParser(description='Generate gtest_verilog TestBench for verilated modules')
parser.add_argument('--name', help="name of testbench to generate", required=True)
parser.add_argument('--verilated-header-dir', help='directory with verilated headers', required=True)
parser.add_argument('--output-source', help='output testbench source file', required=True)
parser.add_argument('--output-header', help='output testbench header file', required=True)

args = parser.parse_args()

verilated_header_filepaths = []

for filename in os.listdir(args.verilated_header_dir):
    if filename.endswith(".h"):
        verilated_header_filepaths.append( os.path.join(args.verilated_header_dir, filename) )

ports = []


verilated_header_filepath = None

for filepath in verilated_header_filepaths:
    with open(filepath, "r") as file:
        for line in file:
            results = re.search("\w*VL_[IN|OUT].*\((.*),(.*),(.*)\);\w*", line.lstrip().rstrip())
            if results:
                # this is the header with port definitions

                # scrape the port definitions
                name = results.group(1)
                hi = results.group(2)
                lo = results.group(3)
                ports.append({
                    "name": name,
                    "hi": hi,
                    "lo": lo,
                    "width": 1 + int(hi) - int(lo)
                })

                verilated_header_filepath = filepath

assert(None != verilated_header_filepath)
assert(len(ports) > 0)

verilated_name = os.path.basename(verilated_header_filepath).rstrip(".h")

namespace = args.name.lower()

def write_source_file():
    source_top = """
#include "{output_header}"

namespace {namespace} {{
""".format(
    output_header = args.output_header,
    namespace = namespace
)

    source_middle = """
    
    void {name}::onNextStep() {{
        using namespace gtestverilog;

        Step step;

""".format(name = args.name)

    source_bottom = """
        trace.append(step);
    }}
}}
""".format(

)

    with open(args.output_source, "w") as file:
        file.write("// my output source\n")

        file.write(source_top)

        for index, port in enumerate(ports):
            file.write(
                "    PORT_DESCRIPTION({index}, {name}, {width});\n"
                .format(
                    index = index,
                    name = port["name"],
                    width = port["width"]
                )
            )

        file.write(source_middle)

        for port in ports:
            if port["width"] == 1:
                file.write(
                    "        step.port({name}) = (core().{name} == 1);\n"
                    .format(
                        name = port["name"],
                    )
                )
            else:
                file.write(
                    "        step.port({name}) = core().{name};\n"
                    .format(
                        name = port["name"],
                    )
                )

        file.write(source_bottom)

def write_header_file():
    header_top = """
#pragma once

#include "gtestverilog/gtestverilog.h"
#include "{verilated_header_filepath}"

namespace {namespace} {{
""".format(
        verilated_header_filepath = verilated_header_filepath,
        namespace=namespace
    )

    header_bottom = """
    class {name} : public ::gtestverilog::TestBench<{verilated_name}> {{
    public:

        virtual void onNextStep() override;

        ::gtestverilog::Trace trace;

    }};
}}

""".format(
    name=args.name,
    verilated_name=verilated_name
)

    with open(args.output_header, "w") as file:
        file.write(header_top)

        for port in ports:
            file.write(
                "    extern ::gtestverilog::PortDescription {port_name};\n"
                .format(port_name = port["name"])
            )

        file.write(header_bottom)

write_source_file()
write_header_file()