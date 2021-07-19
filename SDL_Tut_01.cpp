//Incluimos SDL2 y alguna libreria para el i/o de consola
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

int main (int argn, char* args[]) {
    //Declaramos las dimensiones de la ventana COMO CONSTANTES
    const int W_WIDTH = 640;
    const int W_HEIGHT= 480;

    //Declaramos un puntero a nuestra ventana SDL a un puntero NULLO
    SDL_Window* window = NULL;

    //Declaramos una superficie SDL
    //Esta superficie puede venir de una imágen, u obtenerse de una ventana
    SDL_Surface* surface = NULL;

    //Inicializamos SDL, en caso de que no pueda iniciarse, mandamos un mensaje de error
    if(SDL_Init( SDL_INIT_VIDEO ) < 0) 
        cerr<<" No se puede inicializar SDL: "<<SDL_GetError();
    else{ //Si no hubo problemas, creamos la ventana y le damos una superficie
        window = SDL_CreateWindow( "Tutorial de SDL", 
                                    SDL_WINDOWPOS_UNDEFINED, 
                                    SDL_WINDOWPOS_UNDEFINED, 
                                    W_WIDTH,
                                    W_HEIGHT,
                                    SDL_WINDOW_SHOWN);
        if( window == NULL)
            cerr<<"No se pudo crear la ventana!: "<<SDL_GetError();
        else{
            //Obtenemos la superficie de la ventana
            surface = SDL_GetWindowSurface( window );

            //Modificamos dicha superficie (que está siendo pasada por referencia)
            SDL_FillRect ( surface, NULL, SDL_MapRGB( surface->format, 0xFF, 0xFF, 0xFF ) );
                                /*No rect*//*In the Map function we use the pixel format of the surface*/
            //Actualizamos la ventana con la superficie modificada para ver los cambios hechos
            SDL_UpdateWindowSurface( window );

            //Esperamos unos segundos
            SDL_Delay(2500);
        }

        //Liberamos la memoria de la ventana y cerramos SDL
        SDL_DestroyWindow( window );
        SDL_Quit();
    }
    return 0; //Codigo de retorno del main
}


