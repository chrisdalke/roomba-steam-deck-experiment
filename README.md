# Roomba -> Steam Deck Experiment

This repository holds some files for an experiment in controlling a Roomba using a Steam Deck over
Wifi / UDP. These files are related to this YouTube video: https://www.youtube.com/watch?v=Nc3mK0B79DA&lc=UgzOdXjppEIGzP52XFt4AaABAg

Disclaimer: I was just messing around prototyping. This code is fairly messy and may be hard to use 
for another project unless you're familiar with C/C++ and basic networking.

## Folders

- `esp32`: Code running on the ESP32, which receives UDP packets and outputs as serial data
- `ui`: C++ OpenGL UI designed to run on the Steam Deck, uses Raylib librayr

## Steam Deck Notes

The UI is built natively for linux and loaded onto the Dev kit using the Steam Deck dev tools:
https://partner.steamgames.com/doc/steamdeck/loadgames