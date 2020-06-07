module PC#(parameter WIDTH=32)(
    input logic clk,
    input logic rst,
    input pc_cmd::cmd cmd,
    input logic [WIDTH:0] load_pc,
    output logic [WIDTH:0] pc
);

    logic [WIDTH:0] next_pc;

    always_comb begin
        unique case (cmd)
            pc_cmd::NONE:
                next_pc = pc;
            pc_cmd::INC:
                next_pc = pc+1;
            pc_cmd::LOAD:
                next_pc = load_pc;
            default:
                next_pc = 0;
        endcase

        if (rst == 1)
            next_pc = 0;
    end

    always_ff @(posedge clk) begin
        pc <= next_pc;
    end

endmodule