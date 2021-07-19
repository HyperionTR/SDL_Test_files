#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

//Definimos las constantes
const int w_width = 640;
const int w_height= 480;

//Declaramos las funciones a utilizar
bool init();
bool load();
void exit();

//Declaramos variables gloables
SDL_Window* window = NULL;
SDL_Surface* win_surface = NULL;
SDL_Surface* img_surface = NULL;

int main( int argn, char* args[] ){
    //Verificamos que no halla errores en el programa
    if(!init()){
        cerr<<"Hubo un error durante la inicialización";
        return -1;
    } else if (!load()){
        cerr<<"Hubo un error durante la carga de datos";
        return -2;
    } else{

        //Aplicamos la imagen a la suyperficie
        SDL_BlitSurface( img_surface, NULL, win_surface, NULL);

        //Actualizamos la superficie de nuestra ventana
        SDL_UpdateWindowSurface(window);
        SDL_Delay(2500);

        //Liberamos memoria y salimos del programa
        exit();

        return 0;

    }
}

bool init(){

    bool success = true;

    //Inicializamos SDL y verificamos por errores posibles
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        cerr<<"No ha podido inicializar SDL: "<<SDL_GetError();
        success= false;
    } else{
        //Creamos la ventana SDL
        window = SDL_CreateWindow(  "Tutorial SDL: Eventos",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    w_width,
                                    w_height,
                                    SDL_WINDOW_SHOWN);
        //Verificamos por posibles errores en la ventana
        if( window == NULL){
            cerr<<"No se pudo crear la ventana: "<<SDL_GetError();
            success =  false;
        }else
            //Si no hay errores, obtenemos la superficie de la ventana
            win_surface = SDL_GetWindowSurface(window);
    }
    return success;
}

bool load(){

    //Succes flag
    bool success = true;

    //tratamos de cargar la imagen o mosrtar un error
    const char* file= "./media/CrossCode.bmp";
    img_surface = SDL_LoadBMP(file);

    if (img_surface == NULL){
        cerr<<"No se pudo cargar la imágen: "<<SDL_GetError();
        success = false;
    }

    return success;
}

void exit(){

    //Liberamos las superficies globales utilizadas y destruimos las ventanas
    //Ademas de colocar los punteros usados a nulo
    SDL_FreeSurface(img_surface);
    img_surface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

}