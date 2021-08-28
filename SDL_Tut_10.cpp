#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <iostream>
using namespace std;

//Definimos las constantes
const int w_width = 640;
const int w_height= 480;


//Clase que guarda la información de la textura
class Tut_Texture{
    private: 
        
        int t_width;
        int t_height;
        SDL_Texture* texture;

    public:

        //Constructor
        Tut_Texture(){
            texture = NULL;
            t_width = 0;
            t_height = 0;
        }
        //Destructor
        ~Tut_Texture(){
            free();
        }

        //Carga de la textura al objeto
        bool load_texture( const char* file );
        
        //Liberacion de memoria
        void free();

        //Mostrar la imagen en pantalla
        void render( int x, int y );
        void render( int x, int y, SDL_Rect& crop_rect );

        //Obtener ancho
        int witdh() {
            return t_width;
        }
        //Obtener alto
        int height(){
            return t_height;
        }
};

//Declaramos las funciones a utilizar
bool init();
bool load();
SDL_Surface* load_surface(const char* path);
SDL_Texture* load_texture(const char* path, SDL_Renderer* renderer);
void exit();

//Declaramos variables gloables
SDL_Window* window = NULL;
SDL_Surface* win_surface = NULL;
SDL_Surface* img_surface = NULL;
SDL_Renderer* win_renderer = NULL;
SDL_Texture* img_texture = NULL;
Tut_Texture background;
Tut_Texture player;



int main( int argn, char* args[] ){
    //Verificamos que no halla errores en el programa
    if(!init()){
        cerr<<"Hubo un error durante la inicialización";
        return -1;
    } else if (!load()){
        cerr<<"Hubo un error durante la carga de datos";
        return -2;
    } else{

        //Colocamos nuestra aplicación dentro de un bucle constante llamado main loop
        bool quit = false;
        int posX = w_width/2, posY = 280;
        int s_posX = posX, s_posY = posY;
        SDL_Rect bg_rect = {0, 0, w_width, w_height};
        SDL_Event e;

        /*MAIN LOOP*/
        while (!quit){

            //Manejo de eventos
            while ( SDL_PollEvent(&e) ){
                switch (e.type){
                    case SDL_KEYDOWN:
                        if ( e.key.keysym.sym == SDLK_ESCAPE ){
                            case SDL_QUIT:
                            quit = true;
                            break;
                        }
                        if( e.key.keysym.sym == SDLK_d )
                            posX += 5;
                        else if ( e.key.keysym.sym == SDLK_a )
                            posX -= 5;
                    break;
                }
            }

            //Mostrar una ventana con un renderer dentro con una imagen cargada mediante SDL_Image
            SDL_SetRenderDrawColor( win_renderer, 255, 255, 255, 255);
            SDL_RenderClear(win_renderer);

            background.render(0, 0, bg_rect);

            //Si el jugador está mas atras que la primer mitad de la pantalla 
            if ( posX < bg_rect.w/2 )
                s_posX = posX; // Su pocicion en pantalla es la misma que en el mundo
            else if ( posX > background.witdh() - bg_rect.w/2 ) //Si está mas adelante que la mitad de la pantalla 
                s_posX = posX - bg_rect.x; //Su pocision en pantalla es la diferencia entre la pocision en el mundo y el borde izquiero del marco (que ahora tiene una pocision mayor)
            else{ //Sino
                bg_rect.x = posX - bg_rect.w/2; //La camara se mueve y el jugador esta estatico
                s_posX = bg_rect.w/2;
            }

            if ( posY < bg_rect.h/2 )
                s_posY = posY;

            player.render( s_posX, s_posY );

            SDL_RenderPresent(win_renderer);

        }
        
		//Liberamos memoria y salimos del programa
        exit();
        return 0;

    }
}

bool init(){

    bool success = true;

    //Inicializamos SDL y verificamos por errores posibles
    int img_init_flags = IMG_INIT_PNG;
    if( SDL_Init(SDL_INIT_VIDEO) < 0 && !(IMG_Init(img_init_flags) & img_init_flags) ){
        cerr<<"No ha podido inicializar SDL o SDL_Image "<<endl;
        cerr<<"SDL error: "<<SDL_GetError()<<endl;
        cerr<<"IMG error: "<<IMG_GetError()<<endl;
        success= false;
    } else{
        //Creamos la ventana SDL
        window = SDL_CreateWindow(  "Tutorial SDL: Barebones Template",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    w_width,
                                    w_height,
                                    SDL_WINDOW_SHOWN);
        //Verificamos por posibles errores en la ventana
        if( window == NULL ){
            cerr<<"No se pudo crear la ventana: "<<SDL_GetError();
            success =  false;
        }else

            //Si no hay errores, obtenemos la superficie de la ventana
            win_surface = SDL_GetWindowSurface(window);
            //Y también creamos el renderer de la ventana
            win_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
            if ( win_surface == NULL ){
                cerr<<"No se pudo crear la superficie: "<<SDL_GetError();
                success = false;
            }
            if( win_renderer == NULL ){
                cerr<<"No se ha podido crear el renderer: "<<SDL_GetError();
                success = false;
            } else{
                //Si no hay errores, especificamos el color en blanco opaco
                SDL_SetRenderDrawColor( win_renderer, 255, 255, 255, 255 );
				//E inicializamos nusetras texturas
                background = Tut_Texture();
                player = Tut_Texture();
			}
    }
    return success;
}

bool load(){

    //Succes flag
    bool success = true;

    if ( !background.load_texture("./media/backrgound_2.png") ){
        cerr<<"Hubo un error al cargar el fondo: "<<SDL_GetError();
        success = false;
    }
    if ( !player.load_texture("./media/char.png") ){
        cerr<<"Hubo un error al cargar el personaje: "<<SDL_GetError();
        success = false;
    }

    return success;

}

SDL_Surface* load_surface( const char* path ){
	SDL_Surface* optimized_surface = NULL; //Superficie optimizada para blitting
	SDL_Surface* png_surface = IMG_Load(path); //Imagen cargada mediante SDL_Image
	if( png_surface == NULL){
		cerr<<"No se ha podido cargar la im�gen en "<<path<<":"<<IMG_GetError()<<endl;
	}else{
		//Optimizamos la superficie y regresamos la imagen
		optimized_surface = SDL_ConvertSurface( png_surface, win_surface->format, 0);
		if( optimized_surface == NULL){
			cerr<<"No se ha posido optimizar la superficie: "<<SDL_GetError()<<endl;
		}else{
			SDL_FreeSurface(png_surface);
			return optimized_surface;
		}
	}
	return NULL;
}

SDL_Texture* load_texture( const char* path, SDL_Renderer* renderer ){
    SDL_Surface* img_to_load = load_surface(path);
	SDL_Texture* loaded_texture = NULL;
    if ( img_to_load != NULL ){
        loaded_texture = SDL_CreateTextureFromSurface( renderer, img_to_load );
        if ( loaded_texture == NULL )
			cerr<<"No se ha podido crear la textura con la superficie de "<<path<<": "<<SDL_GetError();
    }
	return loaded_texture;
}

bool Tut_Texture::load_texture( const char* file ){
    free(); //Liberamos memoria
    //Luego hacemos un proceso de carga de imagenes
    SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = load_surface(file);

    if( loaded_surface != NULL ){
        //Si no hay errores, establecemos el color transparente
        SDL_SetColorKey( loaded_surface, SDL_TRUE, SDL_MapRGB( loaded_surface->format, 0, 255, 255 ) );
        new_texture = SDL_CreateTextureFromSurface( win_renderer, loaded_surface );
        if ( new_texture == NULL ){
            cerr<<"Hubo un error durante la carga de la textura: "<<SDL_GetError();
        } else {
            t_width = loaded_surface->w;
            t_height = loaded_surface->h;
        }
        SDL_FreeSurface(loaded_surface);
    }

    texture = new_texture;
    return texture != NULL; //Determina si no es nula la textura
}

void Tut_Texture::render( int x, int y ){
    //Declaramos el rectangulo donde se va a mostrar la textura
    SDL_Rect render_rect = { x, y, t_width, t_height };
    SDL_RenderCopy( win_renderer, texture, NULL, &render_rect );
}

void Tut_Texture::render( int x, int y, SDL_Rect &crop_rect ){
    //Declaramos el rectangulo donde se va a mostrar la textura
    SDL_RenderCopy( win_renderer, texture, &crop_rect, NULL );
}

void Tut_Texture::free(){
    if ( texture != NULL ){
        SDL_DestroyTexture(texture);
        texture = NULL;
        t_width = 0;
        t_height = 0;
    }
}


void exit(){

    //Liberamos las superficies globales utilizadas y destruimos las ventanas
    //Ademas de colocar los punteros usados a nulo
    
    player.free();
    background.free();

    SDL_FreeSurface(img_surface);
    img_surface = NULL;
    
    SDL_FreeSurface(win_surface);
    win_surface = NULL;

    SDL_DestroyRenderer(win_renderer);
    win_renderer = NULL;

    SDL_DestroyTexture(img_texture);
    img_texture = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
    IMG_Quit();
}
