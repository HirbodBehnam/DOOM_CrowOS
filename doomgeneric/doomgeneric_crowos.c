#include "doomkeys.h"
#include "doomgeneric.h"
#include "libc/fb.h"
#include "libc/file.h"
#include "libc/stdio.h"
#include "libc/usyscalls.h"

#define KEYQUEUE_SIZE 16
static uint64_t doom_start_time;
static int framebuffer_fd, async_serial_fd;
static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char convertToDoomKey(unsigned int key) {
    switch (key) {
        case '\n':
            key = KEY_ENTER;
            break;
        case 127: // Backspace
            key = KEY_ESCAPE;
            break;
        case 'a':
        case 'A':
            key = KEY_LEFTARROW;
            break;
        case 'd':
        case 'D':
            key = KEY_RIGHTARROW;
            break;
        case 'w':
        case 'W':
            key = KEY_UPARROW;
            break;
        case 's':
        case 'S':
            key = KEY_DOWNARROW;
            break;
        case 'l':
        case 'L':
            key = KEY_FIRE;
            break;
        case ' ':
            key = KEY_USE;
            break;
        case 'j':
        case 'J':
            key = KEY_RSHIFT;
            break;
        default:
            break;
    }

    return key;
}

static void addKeyToQueue(int pressed, unsigned int keyCode) {
    unsigned char key = convertToDoomKey(keyCode);

    unsigned short keyData = (pressed << 8) | key;

    s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
    s_KeyQueueWriteIndex++;
    s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

void DG_Init() {
    // Open the framebuffer
    framebuffer_fd = open("fb", O_DEVICE);
    if (framebuffer_fd < 0) {
        puts("cannot open framebuffer");
        exit(1);
    }
    // Set the width and height and clear screen
    // Check the screen size
    uint64_t screen_width, screen_height;
    ioctl(framebuffer_fd, FRAMEBUFFER_CTL_GET_MAX_WIDTH, &screen_width);
    ioctl(framebuffer_fd, FRAMEBUFFER_CTL_GET_MAX_HEIGHT, &screen_height);
    if (screen_width < DOOMGENERIC_RESX || screen_height < DOOMGENERIC_RESY) {
        puts("Small screen!");
        exit(1);
    }
    // Clear the screen
    ioctl(framebuffer_fd, FRAMEBUFFER_CTL_CLEAR, NULL);
    // Set the dimensions
    ioctl(framebuffer_fd, FRAMEBUFFER_CTL_SET_WIDTH, (void *) ((uint64_t) DOOMGENERIC_RESX));
    ioctl(framebuffer_fd, FRAMEBUFFER_CTL_SET_HEIGHT, (void *) ((uint64_t) DOOMGENERIC_RESY));

    // Open the serial port
    async_serial_fd = open("serial_async", O_DEVICE);
    if (async_serial_fd < 0) {
        puts("cannot open serial port");
        exit(1);
    }

    puts("DG_Init done");
}

void DG_DrawFrame() {
    // Check the key buffer
    char keypressed_buffer[16];
    int keys_pressed = read(async_serial_fd, keypressed_buffer, sizeof(keypressed_buffer));
    if (keys_pressed != 0) {
        addKeyToQueue(1, keypressed_buffer[keys_pressed - 1]);
        addKeyToQueue(2, keypressed_buffer[keys_pressed - 1]);
        addKeyToQueue(0, keypressed_buffer[keys_pressed - 1]);
    }
    // Draw the frame
    write(framebuffer_fd, DG_ScreenBuffer, DOOMGENERIC_RESX * DOOMGENERIC_RESY);
}

void DG_SleepMs(uint32_t ms) {
    sleep(ms);
}

uint32_t DG_GetTicksMs() {
    uint64_t current_time = time();
    return (uint32_t) (current_time - doom_start_time);
}

int DG_GetKey(int *pressed, unsigned char *doomKey) {
    if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex) {
        //key queue is empty

        return 0;
    } else {
        unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
        s_KeyQueueReadIndex++;
        s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

        if ((keyData >> 8) == 2) {
            return 0;
        }

        *pressed = keyData >> 8;
        *doomKey = keyData & 0xFF;

        return 1;
    }
}

void DG_SetWindowTitle(const char *title) {
}

int main(int argc, char **argv) {
    doom_start_time = time();
    doomgeneric_Create(argc, argv);
    for (int i = 0; ; i++)
        doomgeneric_Tick();

    return 0;
}
