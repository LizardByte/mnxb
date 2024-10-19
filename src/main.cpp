// standard includes
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

// lib includes
#include <SDL.h>
#include <SDL_image.h>

// nxdk includes
#include "src/nxdk/hal/debug.h"
#include "src/nxdk/hal/video.h"
#include "src/nxdk/hal/xbox.h"
#include "src/nxdk/windows.h"

// local includes
#include "src/os.h"

static void printSDLErrorAndReboot() {
  debugPrint("SDL_Error: %s\n", SDL_GetError());
  debugPrint("Rebooting in 5 seconds.\n");
  Sleep(5000);
  XReboot();
}

static void printIMGErrorAndReboot() {
  debugPrint("SDL_Image Error: %s\n", IMG_GetError());
  debugPrint("Rebooting in 5 seconds.\n");
  Sleep(5000);
  XReboot();
}

// Screen dimension globals
static int SCREEN_WIDTH;
static int SCREEN_HEIGHT;

void splash_screen() {
  int done = 0;
  SDL_Window *window;
  SDL_Event event;
  SDL_Surface *screenSurface, *imageSurface;

  // Enable standard application logging
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

  if (SDL_VideoInit(NULL) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL video.\n");
    printSDLErrorAndReboot();
  }

  window = SDL_CreateWindow("splash", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    debugPrint("Window could not be created!\n");
    SDL_VideoQuit();
    printSDLErrorAndReboot();
  }

  if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't intialize SDL_image.\n");
    SDL_VideoQuit();
    printIMGErrorAndReboot();
  }

  screenSurface = SDL_GetWindowSurface(window);
  if (!screenSurface) {
    SDL_VideoQuit();
    printSDLErrorAndReboot();
  }

  // set string variable for splash screen path
  std::string _splashScreenPath = std::string(DATA_PATH) + "assets" + PATH_SEP + "moonlight-splash-" +
                                  std::to_string(SCREEN_WIDTH) + "x" + std::to_string(SCREEN_HEIGHT) + ".jpg";
  const char *splashScreenPath = _splashScreenPath.c_str();

  imageSurface = IMG_Load(splashScreenPath);
  if (!imageSurface) {
    SDL_VideoQuit();
    printIMGErrorAndReboot();
  }

  while (!done) {
    // Check for events
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          done = 1;
          break;
        default:
          break;
      }
    }

    SDL_BlitSurface(imageSurface, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);

    Sleep(1000);
  }

  SDL_VideoQuit();
}

int main() {
  // create an empty list for the available video modes
  std::vector<VIDEO_MODE> availableVideoModes;

  // save the best video mode here
  VIDEO_MODE bestVideoMode = {0, 0, 0, 0};

  VIDEO_MODE vm;
  int bpp = 32;  // Bits per pixel
  void *p = NULL;  // Initialize to NULL for the first call

  // get the available video modes
  while (XVideoListModes(&vm, bpp, REFRESH_DEFAULT, &p)) {
    availableVideoModes.push_back(vm);

    // ensure height is equal to or better than the current best video mode
    if (vm.height < bestVideoMode.height) {
      continue;
    }

    // ensure width is equal to or better than the current best video mode
    if (vm.width < bestVideoMode.width) {
      continue;
    }

    // ensure bpp is equal to or better than the current best video mode
    if (vm.bpp < bestVideoMode.bpp) {
      continue;
    }

    // ensure refresh is equal to or better than the current best video mode
    if (vm.refresh < bestVideoMode.refresh) {
      continue;
    }

    // save the best video mode
    bestVideoMode = vm;
  }

  SCREEN_WIDTH = bestVideoMode.width;
  SCREEN_HEIGHT = bestVideoMode.height;

  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);

  debugPrint("Available video modes:\n");
  for (VIDEO_MODE vm : availableVideoModes) {
    debugPrint("Width: %d, Height: %d, BPP: %d, Refresh: %d\n", vm.width, vm.height, vm.bpp, vm.refresh);
  }

  debugPrint("Best video mode:\n");
  debugPrint("Width: %d, Height: %d, BPP: %d, Refresh: %d\n", bestVideoMode.width, bestVideoMode.height, bestVideoMode.bpp, bestVideoMode.refresh);

  Sleep(2000);

  XVideoSetMode(SCREEN_WIDTH, SCREEN_HEIGHT, bestVideoMode.bpp, bestVideoMode.refresh);

  splash_screen();
  return 0;
}
