module add_one(
    input logic cmd,
    input logic clock,
    input logic reset,
    output logic [32:0] value);

    logic [32:0] v;

    always_comb begin
        if (reset == 1)
            v = 0;

        if (cmd == 1)
            v = value+1;
    end

    always_ff @(posedge clock)
        value <= v;
endmodule: add_one