import argparse
import sys
from os import path

import m5
from m5.objects import *

# construct a simple virtual platform
# with 1 RISC-V CPU

# future work :
# multi-core
# PLIC

# A very simplified platform
class SimpleRiscvPlatform(SimplePlatform):
    def __init__(self):
        super(SimpleRiscvPlatform, self).__init__()

        # RTCCLK (Set to 100MHz for faster simulation)
        self.rtc = RiscvRTC(frequency=Frequency("100MHz"))
        self.clint = Clint(pio_addr=0x2000000, num_threads=1)
        self.clint.int_pin = self.rtc.int_pin

        self.terminal = Terminal()
        self.uart = SimpleUart(pio_addr=0x10000000)

    def devices(self):
        return [
            self.clint,
            self.uart
        ]
    def attachChipIO(self, bus):
        for device in self.devices():
            device.pio = bus.mem_side_ports

# ------------- cache ------------ #
# There caches are from learning_gem5/part1
class L1Cache(Cache):
    """Simple L1 Cache with default values"""
    assoc = 2
    tag_latency = 2
    data_latency = 2
    response_latency = 2
    mshrs = 4
    tgts_per_mshr = 20

# -------------- add options -------------- #
parser = argparse.ArgumentParser()
# some options are from `common/Options.py`
parser.add_argument(
    "--sys-voltage",
    action="store",
    type=str,
    default="1.0V",
    help="""Top-level voltage for blocks running at system
            power supply""",
)
parser.add_argument(
    "--sys-clock",
    action="store",
    type=str,
    default="1GHz",
    help="""Top-level clock for blocks running at system
                speed""",
)
parser.add_argument(
    "--cpu-clock",
    action="store",
    type=str,
    default="2GHz",
    help="Clock for blocks running at CPU speed",
)
parser.add_argument(
    '--firmware',
    dest='firmware_path',
    metavar='FIRMWARE',
    required=True,
    help='The firmware image to run',
)
parser.add_argument(
    '--dram-latency',
    action="store",
    dest='dram_latency',
    required=False,
    default="30ns",
    help='The firmware image to run',
)
parser.add_argument(
    '--cpu-type',
    action="store",
    dest='cpu_type',
    required=False,
    default="atomic",
    choices=['atomic', 'timing', 'minor'],
    help='The firmware image to run',
)
parser.add_argument(
    '--l1-icache',
    action='store_true',
    default=False,
    help='Add a l1-icache',
)
parser.add_argument(
    '--l1-dcache',
    action='store_true',
    default=False,
    help='Add a l1-dcache',
)
parser.add_argument(
    '--dry-run',
    action='store_true',
    default=False,
    help='Enable to skip simulation and simply check hardware layout',
)

# ---------------------------- Parse Options --------------------------- #
args = parser.parse_args()

# ---------------------------- Setup System ---------------------------- #
system = RiscvSystem()

system.membus = SystemXBar()
system.system_port = system.membus.cpu_side_ports

# Create a top-level voltage domain
system.voltage_domain = VoltageDomain(voltage=args.sys_voltage)

# Create a source clock for the system and set the clock period
system.clk_domain = SrcClockDomain(
    clock=args.sys_clock, voltage_domain=system.voltage_domain
)

# Create a CPU voltage domain
system.cpu_voltage_domain = VoltageDomain()

# Create a source clock for the CPUs and set the clock period
system.cpu_clk_domain = SrcClockDomain(
    clock=args.cpu_clock, voltage_domain=system.cpu_voltage_domain
)

if args.cpu_type == "atomic":
    system.cpu = RiscvAtomicSimpleCPU(clk_domain = system.cpu_clk_domain,
                                      cpu_id = 0,
                                      simulate_data_stalls = True,
                                      simulate_inst_stalls = True)
    system.mem_mode = "atomic"
elif args.cpu_type == "timing":
    system.cpu = RiscvTimingSimpleCPU(clk_domain = system.cpu_clk_domain,
                                      cpu_id = 0)
    system.mem_mode = "timing"
elif args.cpu_type == "minor":
    system.cpu = RiscvMinorCPU(clk_domain = system.cpu_clk_domain,
                           cpu_id = 0)
    system.mem_mode = "timing"
else:
    panic("unsupported mem_mode!")

if args.l1_icache:
    system.l1_icache = L1Cache(size='8KiB')
    system.cpu.icache_port = system.l1_icache.cpu_side
    system.l1_icache.mem_side = system.membus.cpu_side_ports
else:
    system.cpu.icache_port = system.membus.cpu_side_ports

if args.l1_dcache:
    system.l1_dcache = L1Cache(size='8KiB')
    system.cpu.dcache_port = system.l1_dcache.cpu_side
    system.l1_dcache.mem_side = system.membus.cpu_side_ports
else:
    system.cpu.dcache_port = system.membus.cpu_side_ports

system.cpu.createThreads()
system.cpu.createInterruptController()

# note :
#     cpu.ArchISA is a class
#     cpu.isa is an object array
#     cpu.isa[0] is the isa for the first thread
#         ( we almost only use one thread for a CPU model )
system.cpu.isa[0].riscv_type = "RV32"

system.platform = SimpleRiscvPlatform()
system.platform.attachChipIO(system.membus)

system.dram = SimpleMemory(
    range=AddrRange(0x8000_0000, size='4GiB'),
    port=system.membus.mem_side_ports,
    in_addr_map=False,
    collect_stats=False,
    latency=args.dram_latency,
)

# XXX : make a args for `wait-for-remote-gdb`
system.workload = RiscvBareMetal(wait_for_remote_gdb=False)
system.workload.bootloader = args.firmware_path

root = Root(full_system=True, system=system)

m5.instantiate()
if args.dry_run:
    print('Dry run is enabled. Skip simulation.')
else:
    sim_exit = m5.simulate(m5.MaxTick)
    print('Exiting @ tick %s,  Exiting %s', m5.curTick(), sim_exit.getCause())
