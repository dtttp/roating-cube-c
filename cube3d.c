#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

/*
   Developed in Spain, although with foreign translation
   The shit cube uses rotations with sines and cosines about the axes (A, B, C are angles)
   Use a z-buffer so the pixels dont go to hell
*/



#ifndef _WIN32
  #include <unistd.h>
#else
  #include <windows.h>
  void usleep(__int64 usec)
  {
    HANDLE timer;
    LARGE_INTEGER ft;
    ft.QuadPart = -(10 * usec);
    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
  }
  void enable_ansi() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD mode;
    if (!GetConsoleMode(hOut, &mode)) return;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
  }
#endif

volatile int running = 1;
void onint(int signum) {
    (void)signum;
    running = 0;
}

int main(void) {
    signal(SIGINT, onint);
    signal(SIGTERM, onint);

  #ifdef _WIN32
    enable_ansi();
  #endif

    setvbuf(stdout, NULL, _IONBF, 0);

    const int width  = 120;
    const int height = 30;

    float cubeWidth = 20.0f;       
    float incrementSpeed = 1.0f;  

    int size = width * height;
    float *zBuffer = (float*)malloc(sizeof(float) * size);
    char  *buffer  = (char*) malloc(sizeof(char)  * size);
    if (!zBuffer || !buffer) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }

    const char background = ' ';

    const int distanceFromCam = 100;
    const float K1 = 40.0f;

    float A = 0.0f, B = 0.0f, C = 0.0f;
    const float dA = 0.018f;
    const float dB = 0.014f;
    const float dC = 0.009f;

    const int frame_us = 16667; 

    printf("\x1b[2J");         
    printf("\x1b[?25l");       

    while (running) {
        for (int i = 0; i < size; ++i) {
            buffer[i] = background;
            zBuffer[i] = 0.0f;
        }

        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
                {
                    float i = cubeX, j = cubeY, k = -cubeWidth;
                    float x = j * sinf(A) * sinf(B) * cosf(C)
                            - k * cosf(A) * sinf(B) * cosf(C)
                            + j * cosf(A) * sinf(C)
                            + k * sinf(A) * sinf(C)
                            + i * cosf(B) * cosf(C);
                    float y = j * cosf(A) * cosf(C)
                            + k * sinf(A) * cosf(C)
                            - j * sinf(A) * sinf(B) * sinf(C)
                            + k * cosf(A) * sinf(B) * sinf(C)
                            - i * cosf(B) * sinf(C);
                    float z = k * cosf(A) * cosf(B)
                            - j * sinf(A) * cosf(B)
                            + i * sinf(B) + distanceFromCam;
                    float ooz = 1.0f / z;
                    int xp = (int)(width / 2 + K1 * ooz * x * 2.0f);
                    int yp = (int)(height / 2 + K1 * ooz * y);
                    if (xp >= 0 && xp < width && yp >= 0 && yp < height) {
                        int idx = xp + yp * width;
                        if (ooz > zBuffer[idx]) { zBuffer[idx] = ooz; buffer[idx] = '@'; }
                    }
                }
                {
                    float i = cubeWidth, j = cubeY, k = cubeX;
                    float x = j * sinf(A) * sinf(B) * cosf(C)
                            - k * cosf(A) * sinf(B) * cosf(C)
                            + j * cosf(A) * sinf(C)
                            + k * sinf(A) * sinf(C)
                            + i * cosf(B) * cosf(C);
                    float y = j * cosf(A) * cosf(C)
                            + k * sinf(A) * cosf(C)
                            - j * sinf(A) * sinf(B) * sinf(C)
                            + k * cosf(A) * sinf(B) * sinf(C)
                            - i * cosf(B) * sinf(C);
                    float z = k * cosf(A) * cosf(B)
                            - j * sinf(A) * cosf(B)
                            + i * sinf(B) + distanceFromCam;
                    float ooz = 1.0f / z;
                    int xp = (int)(width / 2 + K1 * ooz * x * 2.0f);
                    int yp = (int)(height / 2 + K1 * ooz * y);
                    if (xp >= 0 && xp < width && yp >= 0 && yp < height) {
                        int idx = xp + yp * width;
                        if (ooz > zBuffer[idx]) { zBuffer[idx] = ooz; buffer[idx] = '$'; }
                    }
                }
                {
                    float i = -cubeWidth, j = cubeY, k = -cubeX;
                    float x = j * sinf(A) * sinf(B) * cosf(C)
                            - k * cosf(A) * sinf(B) * cosf(C)
                            + j * cosf(A) * sinf(C)
                            + k * sinf(A) * sinf(C)
                            + i * cosf(B) * cosf(C);
                    float y = j * cosf(A) * cosf(C)
                            + k * sinf(A) * cosf(C)
                            - j * sinf(A) * sinf(B) * sinf(C)
                            + k * cosf(A) * sinf(B) * sinf(C)
                            - i * cosf(B) * sinf(C);
                    float z = k * cosf(A) * cosf(B)
                            - j * sinf(A) * cosf(B)
                            + i * sinf(B) + distanceFromCam;
                    float ooz = 1.0f / z;
                    int xp = (int)(width / 2 + K1 * ooz * x * 2.0f);
                    int yp = (int)(height / 2 + K1 * ooz * y);
                    if (xp >= 0 && xp < width && yp >= 0 && yp < height) {
                        int idx = xp + yp * width;
                        if (ooz > zBuffer[idx]) { zBuffer[idx] = ooz; buffer[idx] = '~'; }
                    }
                }
                {
                    float i = -cubeX, j = cubeY, k = cubeWidth;
                    float x = j * sinf(A) * sinf(B) * cosf(C)
                            - k * cosf(A) * sinf(B) * cosf(C)
                            + j * cosf(A) * sinf(C)
                            + k * sinf(A) * sinf(C)
                            + i * cosf(B) * cosf(C);
                    float y = j * cosf(A) * cosf(C)
                            + k * sinf(A) * cosf(C)
                            - j * sinf(A) * sinf(B) * sinf(C)
                            + k * cosf(A) * sinf(B) * sinf(C)
                            - i * cosf(B) * sinf(C);
                    float z = k * cosf(A) * cosf(B)
                            - j * sinf(A) * cosf(B)
                            + i * sinf(B) + distanceFromCam;
                    float ooz = 1.0f / z;
                    int xp = (int)(width / 2 + K1 * ooz * x * 2.0f);
                    int yp = (int)(height / 2 + K1 * ooz * y);
                    if (xp >= 0 && xp < width && yp >= 0 && yp < height) {
                        int idx = xp + yp * width;
                        if (ooz > zBuffer[idx]) { zBuffer[idx] = ooz; buffer[idx] = '#'; }
                    }
                }
                {
                    float i = cubeX, j = -cubeWidth, k = -cubeY;
                    float x = j * sinf(A) * sinf(B) * cosf(C)
                            - k * cosf(A) * sinf(B) * cosf(C)
                            + j * cosf(A) * sinf(C)
                            + k * sinf(A) * sinf(C)
                            + i * cosf(B) * cosf(C);
                    float y = j * cosf(A) * cosf(C)
                            + k * sinf(A) * cosf(C)
                            - j * sinf(A) * sinf(B) * sinf(C)
                            + k * cosf(A) * sinf(B) * sinf(C)
                            - i * cosf(B) * sinf(C);
                    float z = k * cosf(A) * cosf(B)
                            - j * sinf(A) * cosf(B)
                            + i * sinf(B) + distanceFromCam;
                    float ooz = 1.0f / z;
                    int xp = (int)(width / 2 + K1 * ooz * x * 2.0f);
                    int yp = (int)(height / 2 + K1 * ooz * y);
                    if (xp >= 0 && xp < width && yp >= 0 && yp < height) {
                        int idx = xp + yp * width;
                        if (ooz > zBuffer[idx]) { zBuffer[idx] = ooz; buffer[idx] = ';'; }
                    }
                }
                {
                    float i = cubeX, j = cubeWidth, k = cubeY;
                    float x = j * sinf(A) * sinf(B) * cosf(C)
                            - k * cosf(A) * sinf(B) * cosf(C)
                            + j * cosf(A) * sinf(C)
                            + k * sinf(A) * sinf(C)
                            + i * cosf(B) * cosf(C);
                    float y = j * cosf(A) * cosf(C)
                            + k * sinf(A) * cosf(C)
                            - j * sinf(A) * sinf(B) * sinf(C)
                            + k * cosf(A) * sinf(B) * sinf(C)
                            - i * cosf(B) * sinf(C);
                    float z = k * cosf(A) * cosf(B)
                            - j * sinf(A) * cosf(B)
                            + i * sinf(B) + distanceFromCam;
                    float ooz = 1.0f / z;
                    int xp = (int)(width / 2 + K1 * ooz * x * 2.0f);
                    int yp = (int)(height / 2 + K1 * ooz * y);
                    if (xp >= 0 && xp < width && yp >= 0 && yp < height) {
                        int idx = xp + yp * width;
                        if (ooz > zBuffer[idx]) { zBuffer[idx] = ooz; buffer[idx] = '+'; }
                    }
                }
            }
        }

        printf("\x1b[H");
        for (int y = 0; y < height; ++y) {
            int base = y * width;
            for (int x = 0; x < width; ++x) putchar(buffer[base + x]);
            putchar('\n');
        }

        A += dA;
        B += dB;
        C += dC;

    #ifdef _WIN32
        usleep(frame_us);
    #else
        usleep(frame_us);
    #endif
    }

    printf("\x1b[?25h\x1b[2J");
    free(zBuffer);
    free(buffer);
    return 0;
}
