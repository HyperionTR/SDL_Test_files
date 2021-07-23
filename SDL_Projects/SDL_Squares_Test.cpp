#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
using namespace std;

const int win_w = 640;
const int win_h = 480;

int main( int argn, char* args[] ){

    SDL_Window* main_window = SDL_CreateWindow( "SDL Test: Cuadrados",
                                                SDL_WINDOWPOS_UNDEFINED,
                                                SDL_WINDOWPOS_UNDEFINED,
                                                win_w,win_h,
                                                SDL_WINDOW_SHOWN);
    SDL_Surface* win_surface = SDL_GetWindowSurface(main_window);
    bool quit = false;
    SDL_Event e;

    SDL_Rect player;
    player.w = 40;
    player.h = 40;
    Uint32 player_color = SDL_MapRGB( win_surface-> format, 255,0,0);
    double p_sinval = 0;

    SDL_Rect box;
    box.w = 100;
    box.h = 100;
    box.x = win_w/2 - box.w/2;
    box.y= win_h/2 - box.h/2;

    SDL_Rect intersection;

    while (!quit){
        
        //Event manager
        while (SDL_PollEvent(&e)){
            switch (e.type){
                case SDL_MOUSEMOTION:
                player.x = e.motion.x - (player.w/2);
                player.y = e.motion.y - (player.h/2);
                break;
                case SDL_KEYDOWN:
                if( e.key.keysym.sym == SDLK_ESCAPE){
                    case SDL_QUIT:
                    quit = true;
                    break;
                }
                switch (e.key.keysym.sym){
                    case SDLK_UP:
                        box.y-=5;
                        break;
                    case SDLK_DOWN:
                        box.y+=5;
                        break;
                    case SDLK_LEFT:
                        box.x-=5;
                        break;
                    case SDLK_RIGHT:
                        box.x+=5;
                        break;
                }
            }
        }

        p_sinval = int(p_sinval+1)%31415;

        SDL_FillRect( win_surface, NULL, SDL_MapRGB( win_surface->format, 0, 0, 0) );
        SDL_FillRect( win_surface, &box, SDL_MapRGB( win_surface->format, 0, 255, 0) );
        SDL_FillRect( win_surface, &player, player_color);

        //Esto solo comprueba si hay una intersección entre ambos
        if( SDL_IntersectRect(&box, &player, &intersection) ){
            player_color = SDL_MapRGB( win_surface->format, 0,0,255 );
            Uint32 inter_color = SDL_MapRGB( win_surface->format, abs(sin(p_sinval*0.0001)*255), abs(cos(p_sinval*0.00018)*255), abs(sin(p_sinval*0.0007)*255) );
            SDL_FillRect( win_surface, &intersection, inter_color);
        }
        else
            player_color = SDL_MapRGB( win_surface->format, 255,0,0 );



        SDL_UpdateWindowSurface(main_window);
    }

}
