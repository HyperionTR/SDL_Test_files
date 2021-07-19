//Segundo programa de creación de videojuegos: Poner una imagen en la pantalla
//Libreriías
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

//Declaramos las constantes globales del programa
const int w_width = 640;
const int w_height= 480;

//Declaración de las funciones a utilizar. las primeras 3 son bool para poder detectar errores dentro de ellas

bool init(); //función de inicialización de objetos SDL
bool load(); //Función para cargar datos de archivos
void exit(); //Funciíon para salid y limpiar datos de SDL

/*!!! Variables gloables !!!*/
SDL_Window* window = NULL;
SDL_Surface* w_surface = NULL; //La superficie de la ventana a mostrar
SDL_Surface* f_surface = NULL; //La superficie oibtenida de una imagen

int main(int argn, char* args[]){
    
    if(!init())
        cerr<<"Hubo algun error en la inizialización";
    else if (!load())
        cerr<<"Hubo algun error durante la carga de datos";
    else{
        //Colocamos un rectangulo de color en la ventana
        SDL_FillRect( w_surface, NULL, SDL_MapRGB(w_surface->format,0, 8, 150) );
        //Colocamos la imagen en la superficie de la ventana
        SDL_BlitSurface(f_surface, NULL, w_surface, NULL);

        //Mostramos la imágen en la ventana y esperamos unos segundos
        SDL_UpdateWindowSurface( window );
        SDL_Delay(2500);

        //Salimos de SDL
        exit();
        return 0;
    }
        
    
    return 0;
}

bool init(){

    //Variable de éxito
    bool success = true;

    //Inicializamos el video SDL
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        cerr<<"No se pudo inicializar SDL: "<<SDL_GetError();
        success = false; //Declaramos que hubo un error.
    }else{
        //Creamos la ventena y detectamos errores en ello
        window = SDL_CreateWindow( "Tutorial SDL: Imagen en una ventana",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    w_width,
                                    w_height,
                                    SDL_WINDOW_SHOWN);
        if( window == NULL ){
            cerr<<"No se pudo crear la ventana: "<<SDL_GetError();
            success =  false;
        }else
            w_surface = SDL_GetWindowSurface( window );
    }
    return success;
}

bool load(){
    bool success = true;

    //Cargamos una imagen BMP en una superficie
    const char* file= "./media/CrossCode.bmp";
    f_surface = SDL_LoadBMP("./media/CrossCode.bmp");
    if( f_surface == NULL){
        cerr<<"No se pudo cargar la imágen: "<<SDL_GetError();
        success = false;
    }

    return success;
}

void exit(){
    //Liberar la superficie con la imagen
    SDL_FreeSurface( f_surface );
    f_surface = NULL;
    
    //Destruimos la ventana SDL
    SDL_DestroyWindow( window );
    window = NULL;

    SDL_Quit();
}