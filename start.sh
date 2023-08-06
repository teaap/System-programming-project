ASSEMBLER=./assembler

LINKER=./linker
EMULATOR=./emulator
TEST=./tests/

${ASSEMBLER} -o main.o ${TEST}main.s
${ASSEMBLER} -o math.o ${TEST}math.s
${ASSEMBLER} -o ivt.o ${TEST}ivt.s
${ASSEMBLER} -o isr_reset.o ${TEST}isr_reset.s
${ASSEMBLER} -o isr_terminal.o ${TEST}isr_terminal.s
${ASSEMBLER} -o isr_timer.o ${TEST}isr_timer.s
${ASSEMBLER} -o isr_user0.o ${TEST}isr_user0.s
${LINKER} -hex -o program.hex ivt.o math.o main.o isr_reset.o isr_terminal.o isr_timer.o isr_user0.o
${EMULATOR} program.hex