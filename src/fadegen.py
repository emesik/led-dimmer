#!/usr/bin/python
#
# Generate a table of PWM duty cycle to obtain linear brightness change

TABLE_SIZE = 0x100
MAX_VAL = 40

def calc(v):
	v = float(v * 100)
	if v < 8:
		return v/902.3
	else:
		return ((v+16.0)/116.0)**3

tab = [int(round(calc(float(x)/TABLE_SIZE)*MAX_VAL)) for x in range(0, TABLE_SIZE)]

print "/* LED brightness correction table */"
print "uint8_t pot2pwm[%d] = {" % TABLE_SIZE
for i, v in enumerate(tab):
	if i % 10 == 0:
		print "\t",
	print "%2d," % v,
	if i % 10 == 9:
		print "\n"
print "\t};"
