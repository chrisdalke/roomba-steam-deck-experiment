#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <vector>
#include <memory>
#include <string>
#include <math.h>
#include <cstring>
#include <iostream>
#include <locale>
#include <chrono>
#include <stdlib.h>
#include <fmt/core.h>
#include <unistd.h>
#include <csignal>

uint64_t getCurrentMillis() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int sock;
unsigned char multicastTTL = 32;
sockaddr_in groupSock;
in_addr localIface; 
uint64_t lastPublish;
bool running = true;
Font font1;
Font font2;
bool isActive = false;

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";
   exit(0);  
}
void initSocket() {
    sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&groupSock, 0, sizeof(groupSock));
    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr("224.3.29.71");
    groupSock.sin_port = htons(5007);

    //setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &multicastTTL, sizeof(multicastTTL));
}

void sendPacket(std::vector<int>& values) {
    std::cout << "Sending packet: [";
    for (auto val: values) {
        std::cout << val << " ";
    }
    std::cout << "]" << std::endl;
    std::string valueStr = std::string{values.begin(),values.end()};
    sendto(sock, valueStr.c_str(), values.size(), 0, (struct sockaddr *) &groupSock, sizeof(groupSock));
}

bool drawButton(std::string label, int x, int y, int width, int height) {
    Vector2 mousePos = GetMousePosition();
    DrawRectangle(x, y, width, height, (Color){0,0,0,128});
    DrawTextEx(font1, label.c_str(), (Vector2){x + 30, y + (height/2) - 12}, 24, 1, (Color){255,255,255,220});

    if (
        mousePos.x >= x &&
        mousePos.y >= y &&
        mousePos.x <= (x + width) &&
        mousePos.y <= (y + height)) {
        if (IsMouseButtonDown(0)) {
            DrawRectangle(x, y, width, height, (Color){81, 214, 117 ,80});
        }
        if (IsMouseButtonReleased(0)) {
            std::cout << fmt::format("Button pressed: {}", label) << std::endl;
            return true;
        } else {
            return false;
        }
    }
    return false;
}

int main(int argc, char** argv){
    signal(SIGINT, signalHandler);  
    signal(SIGTERM, signalHandler);  

    int screenWidth = 1280;
    int screenHeight = 800;

    //SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Steam Deck Roomba Controller");
    SetTargetFPS(60);

    Texture2D bg = LoadTexture("Resources/bg.png");
    Texture2D fg = LoadTexture("Resources/fg.png");
    SetTextureFilter(bg, TEXTURE_FILTER_BILINEAR);

    Texture2D roomba = LoadTexture("Resources/roomba.png");
    SetTextureFilter(roomba, TEXTURE_FILTER_BILINEAR);

    font1 = LoadFontEx("Resources/Heavitas.ttf", 24, NULL, 0);
    font2 = LoadFontEx("Resources/Heavitas.ttf", 32, NULL, 0);

    initSocket();

    float leftWheel = 0;
    float rightWheel = 0;
    std::vector<int> packet;

    int16_t leftWheelPWM = -255;
    int16_t rightWheelPWM = 255;
    float leftAxis;
    float rightAxis;

    // Init UDP multicast socket
    while (!WindowShouldClose() && running) {
        Vector2 mousePos = GetMousePosition();
        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexture(bg,0,0,WHITE);

        if (IsGamepadAvailable(0)) {
            leftAxis = -1.0f * GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
            rightAxis = -1.0f * GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);

        } else {
            if (IsKeyDown(KEY_LEFT)) {
                leftAxis = 1.0f;
            } else {
                leftAxis = 0.0f;
            }
            if (IsKeyDown(KEY_RIGHT)) {
                rightAxis = 1.0f;
            } else {
                rightAxis = 0.0f;
            }
        }
        leftWheelPWM = (int16_t)(leftAxis * 255);
        rightWheelPWM = (int16_t)(rightAxis * 255);

        bool abort = false;
        if (
            mousePos.x >= 242 &&
            mousePos.y >= 635 &&
            mousePos.x <= (242 + 364) &&
            mousePos.y <= (635 + 132)) {
            if (IsMouseButtonReleased(0)) {
                abort = true;
            }
        }

        int y = 30;
        if (drawButton("Take Control", 860, y, 400, 110) || IsGamepadButtonReleased(0,GAMEPAD_BUTTON_RIGHT_FACE_UP)) {
            packet.clear();
            packet.push_back(128);
            packet.push_back(132);
            sendPacket(packet);
            isActive = true;
        }
        y+= 120;
        if (drawButton("Release Control", 860, y, 400, 110) || IsGamepadButtonReleased(0,GAMEPAD_BUTTON_RIGHT_FACE_LEFT) || abort) {
            packet.clear();
            packet.push_back(173);
            sendPacket(packet);
            isActive = false;
        }
        y+= 120;
        if (drawButton("Start Vacuum", 860, y, 400, 90) || IsGamepadButtonReleased(0,GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) ) {
            packet.clear();
            packet.push_back(138);
            packet.push_back(7);
            sendPacket(packet);
            
        }
        y+= 100;
        if (drawButton("Stop Vacuum", 860, y, 400, 90) || IsGamepadButtonReleased(0,GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
            packet.clear();
            packet.push_back(138);
            packet.push_back(0);
            sendPacket(packet);
            
        }
        y+= 100;

        if (drawButton("Reset", 860, y, 400, 90)) {
            packet.clear();
            packet.push_back(7);
            sendPacket(packet);
            
        }
        y+= 100;
        if (drawButton("Start Docking", 860, y, 400, 90)) {
            packet.clear();
            packet.push_back(143);
            sendPacket(packet);
        }
        y+= 100;
        if (drawButton("Start Cleaning", 860, y, 400, 90)) {
            packet.clear();
            packet.push_back(135);
            sendPacket(packet);
        }

        if (getCurrentMillis() - lastPublish > 100) {
            std::cout << "Sending drive packet" << std::endl;
            lastPublish = getCurrentMillis();

            // Publish drive payload
            packet.clear();
            packet.push_back(146);
            packet.push_back((rightWheelPWM >> 8) & 0xff);
            packet.push_back(rightWheelPWM & 0xff);
            packet.push_back((leftWheelPWM >> 8) & 0xff);
            packet.push_back(leftWheelPWM & 0xff);
            sendPacket(packet);
        }


        int leftWheelHeight = (int16_t)(leftAxis * 240);
        int rightWheelHeight = (int16_t)(rightAxis * 240);

        if (leftWheelPWM > 0) {
            DrawRectangle(66, screenHeight/2 - leftWheelHeight + 2, 143, leftWheelHeight, (Color){81, 214, 117,128});

        } else {
            DrawRectangle(66, screenHeight/2 + 2, 143, leftWheelHeight * -1, (Color){81, 214, 117,128});
        }
        if (rightWheelPWM > 0) {
            DrawRectangle(633, screenHeight/2 - rightWheelHeight + 2, 143, rightWheelHeight, (Color){81, 214, 117,128});

        } else {
            DrawRectangle(633, screenHeight/2 + 2, 143, rightWheelHeight * -1, (Color){81, 214, 117, 128});
        }
        DrawTextEx(font2, fmt::format("{:.0f}", leftAxis * 100.0).c_str(), (Vector2){100 - 16, 700 + 8}, 32, 1, (Color){255,255,255,220});
        DrawTextEx(font2, fmt::format("{:.0f}", rightAxis * 100.0).c_str(), (Vector2){670 - 16, 700 + 8}, 32, 1, (Color){255,255,255,220});
        DrawTexture(fg,0,0,WHITE);
        if (isActive) {
        DrawTextEx(font2, "Status: Active", (Vector2){30, 26}, 32, 1, (Color){255,255,255,255});

        } else {

        DrawTextEx(font2, "Status: Inactive", (Vector2){30, 26}, 32, 1, (Color){255,255,255,255});
        }
        
        EndDrawing();

    }

    std::cout << "Exited" << std::endl;

    CloseWindow();
    return 0;
}
