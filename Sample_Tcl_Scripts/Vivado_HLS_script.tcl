############################################################
## David Berrazueta & Byron Navas
##
## Script to recreate Matrixmul reference design (sample)
## Please DO NOT edit it.
############################################################

# Present a message
puts "Select an IP: "
puts "1) Matrixmul"

# Get input from standard input (keyboard) and store in $option
gets stdin option

# Standard simple if with else clause
if {$option==1} {
    puts "You have selected $option) Matrixmul."
    cd ./Matrixmul
    set fp [open script_HLS.tcl w]
    puts $fp "open_project -reset Matrixmul_HLS_IP"
    puts $fp "set_top matrixmul"
    puts $fp "add_files matrixmul.h"
    puts $fp "add_files matrixmul.cpp"
    puts $fp "add_files -tb matrixmul_test.cpp"
    puts $fp "add_files -tb gold.dat"
    set prj_name "Matrixmul_SoC"
    set top_level "matrixmul"
    set top_level2 "matrixmul_0"
    set axi_name "MATRIXMUL_PERIPH_BUS"
}


if {$option!=1} {
    puts "This sample only works with Matrixmul, please select Option 1"
    exit    
} 


puts $fp "open_solution -reset solution1"

puts "Select a device: "
puts "1) Zynq-7000 SoC ZC702 Evaluation Kit"
puts "2) ZedBoard"
puts "3) Another"
gets stdin device

if {$device==1} {
   puts $fp "set_part {xc7z020clg484-1}"
   set fpga_part "xc7z020clg484-1"
   set board_part "xilinx.com:zc702:part0:1.2"
} elseif {$device==2} {
   puts $fp "set_part {xc7z020clg484-1}"
   set fpga_part "xc7z020clg484-1"
   set board_part "em.avnet.com:zed:part0:1.3"
} else {
   puts "Enter device code: "
   gets stdin device
   puts $fp "set_part {$device}"
   set fpga_part "$device"
   puts "Enter board part: "
   gets stdin board_part
}

puts $fp "create_clock -period 10 -name default"
puts $fp "source directives.tcl"

puts $fp "csim_design"
puts $fp "csynth_design"
puts $fp "cosim_design"
puts $fp "export_design -format ip_catalog"
#puts $fp "exit"

close $fp
cd ./
source "script_HLS.tcl"





set fp [open script_SoC_pre.tcl w]
puts $fp "create_project $prj_name ./$prj_name -part $fpga_part"
puts $fp "set_property board_part $board_part \[current_project\]"
puts $fp "set_property  ip_repo_paths  ./ \[current_project\]"
puts $fp "update_ip_catalog"
puts $fp "create_bd_design \"Zynq_design\""
puts $fp "startgroup"
puts $fp "create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0"
puts $fp "endgroup"
puts $fp "apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config \{make_external \"FIXED_IO, DDR\" apply_board_preset \"1\" Master \"Disable\" Slave \"Disable\" \}  \[get_bd_cells processing_system7_0\]"
puts $fp "startgroup"
puts $fp "set_property -dict \[list CONFIG.PCW_USE_FABRIC_INTERRUPT \{1\} CONFIG.PCW_IRQ_F2P_INTR \{1\} CONFIG.PCW_QSPI_GRP_SINGLE_SS_ENABLE \{1\} CONFIG.PCW_TTC0_PERIPHERAL_ENABLE \{0\}\] \[get_bd_cells processing_system7_0\]"
puts $fp "endgroup"
puts $fp "startgroup"
puts $fp "create_bd_cell -type ip -vlnv xilinx.com:hls:$top_level:1.0 $top_level2"
puts $fp "endgroup"
puts $fp "apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config \{Master \"/processing_system7_0/M_AXI_GP0\" Clk \"Auto\" \}  \[get_bd_intf_pins $top_level2/s_axi_$axi_name\]"
puts $fp "connect_bd_net \[get_bd_pins $top_level2/interrupt\] \[get_bd_pins processing_system7_0/IRQ_F2P\]"
puts $fp "validate_bd_design"
puts $fp "save_bd_design"
puts $fp "generate_target all \[get_files  ./$prj_name/$prj_name.srcs/sources_1/bd/Zynq_design/Zynq_design.bd\]"
puts $fp "export_ip_user_files -of_objects \[get_files ./$prj_name/$prj_name.srcs/sources_1/bd/Zynq_design/Zynq_design.bd\] -no_script -force -quiet"
puts $fp "export_simulation -of_objects \[get_files ./$prj_name/$prj_name.srcs/sources_1/bd/Zynq_design/Zynq_design.bd\] -directory ./$prj_name/$prj_name.ip_user_files/sim_scripts -force -quiet"
puts $fp "make_wrapper -files \[get_files ./$prj_name/$prj_name.srcs/sources_1/bd/Zynq_design/Zynq_design.bd\] -top"
puts $fp "add_files -norecurse ./$prj_name/$prj_name.srcs/sources_1/bd/Zynq_design/hdl/Zynq_design_wrapper.v"
puts $fp "update_compile_order -fileset sources_1"
puts $fp "update_compile_order -fileset sim_1"
puts $fp "launch_runs impl_1 -to_step write_bitstream -jobs 2"
close $fp

set fp [open script_SoC_post.tcl w]
puts $fp "file mkdir ./$prj_name/$prj_name.sdk"
puts $fp "file copy -force ./$prj_name/$prj_name.runs/impl_1/Zynq_design_wrapper.sysdef ./$prj_name/$prj_name.sdk/Zynq_design_wrapper.hdf"
puts $fp "launch_sdk -workspace ./$prj_name/$prj_name.sdk -hwspec ./$prj_name/$prj_name.sdk/Zynq_design_wrapper.hdf"
close $fp

puts "###################################################################"
puts "############## The process has finished successfully ##############"
puts "###################################################################"

exit