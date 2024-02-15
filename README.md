# Tibber Pulse Local ESPHOME

Query the Tibber pulse bridge locally without the web API of Tibber.

If you are looking for a solution to query the Tibber pulse bridge locally without the web API of Tibber and want to get this data into Home Assistant, there is a great solution [here](https://github.com/marq24/ha-tibber-pulse-local).

## Prerequisite

Enable the web frontend of the Tibber bridge.
See [here](https://github.com/ProfDrYoMan/tibber_pulse_local#always-enabling-the-web-frontend) for instructions.

## How to use

Place the file ```TibberLocal.hpp``` in your ESPHOME config directory.

Add the info in ```TibberLocal.yaml``` into a config of the ESP you want to use.
This yaml only holds the necessary info.

Adapt the parameters in line 19.

The first parameter in that line is the polling time in milliseconds. 1000 ms is fine as  the code avoids posting the same measurements over and over if nothing did change. Change only if you know what you are doing.

Parameters 3, 4, and 5 are URL, user, and password. Change according to your needs.

You might also need to adapt the GPIOs for the UART in line 23 and 24.

For the configuration of the SML component check the original documentation [here](https://esphome.io/components/sml.html).

Finally connect both selected UART pins electrically.

## Debug

You can check the ESPHOME debug output and will hopefully figure out what is wrong if it's not working as intended.

Good luck!
