ARDUINO_CLI=arduino-cli
BOARD=arduino:avr:uno
SERIAL_PORT=COM3

.PHONY: all
all: compile

.PHONY: compile
compile:
	$(ARDUINO_CLI) compile -b $(BOARD)

.PHONY: upload
upload:
	$(ARDUINO_CLI) compile -b $(BOARD) -u -p $(SERIAL_PORT)

.PHONY: monitor
monitor:
	$(ARDUINO_CLI) monitor -p $(SERIAL_PORT)