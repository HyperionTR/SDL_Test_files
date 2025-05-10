#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

//Definimos las constantes
const int w_width = 640;
const int w_height= 480;
const int wood_max = 4500;
const int parts_max = 8000;
const int cell_spawn = 10;

//Declaramos las funciones a utilizar
bool init();
bool load();
// SDL_Surface* load_surface(const char* path);
void exit();
 
//Declaramos variables gloables
SDL_Window* window = NULL;
// SDL_Surface* win_surface = NULL;
SDL_Surface* img_surface = NULL;
SDL_Renderer* win_renderer = NULL;
int part_matrix[480][640];

//Declaramos enumeraciones
enum cell_type{
	CELL_SAND,
	CELL_SMOKE,
	CELL_WOOD
};

//Declaramos un objeto
class Cell{
	public:
	float x;
	float y;
	int type;

	Cell(float x_, float y_, int type_){
		x = x_;
		y = y_;
		type = type_;
	}
	
	void rules(){
		switch (type){   
		case CELL_SAND:
			if ( part_matrix[int(y+1)][int(x)] ){
				if (part_matrix[int(y+1)][int(x-1)] && !part_matrix[int(y+1)][int(x+1)]){
					part_matrix[(int)y][(int)x] = false;
					x++;
				}
				else if (part_matrix[int(y+1)][int(x+1)] && !part_matrix[int(y+1)][int(x-1)]){
					part_matrix[(int)y][(int)x] = false;
					x--;  
				}else if (!part_matrix[int(y+1)][int(x+1)] && !part_matrix[int(y+1)][int(x-1)])
					x = rand()%2? x+1 : x-1;
			}
			else if( y < 400 ){  
				y++;
			}
			break;
		case CELL_SMOKE:
			if( y > 50 && !part_matrix[int(y-1)][int(x)] ){
				y--;
				if ( !part_matrix[int(y)][int(x+1)] && !part_matrix[int(y)][int(x-1)] )
					x = rand()%2? x+1 : x-1;
			}
			else if( !part_matrix[int(y)][int(x+1)] && part_matrix[int(y)][int(x-1)] )
				x++;
			else if( !part_matrix[int(y)][int(x-1)] && part_matrix[int(y)][int(x+1)])
				x--;
			else if ( !part_matrix[int(y)][int(x+1)] && !part_matrix[int(y)][int(x-1)] )
				x = rand()%2? x+1 : x-1;
			break;
		default:
		case CELL_WOOD:
			break;
		}
	}
	~Cell(){
	}
};

int main( int argn, char* args[] ){
    //Verificamos que no halla errores en el programa
    if(!init()){
        cerr<<"Hubo un error durante la inicialización";
        return -1;
    } else if (!load()){
        cerr<<"Hubo un error durante la carga de datos";
        return -2;
    } else{

		//Iniciamos el ciclo principal
 		bool quit = false;
 		SDL_Event e;
 		int cposX = 0, cposY = 0;
 		vector<Cell> particulas;
 		vector<Cell> wood;
 		//SDL_Delay(5500);
 		while (!quit){
 			
 			while ( SDL_PollEvent(&e) ){
 				if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
 					quit = true;
 				if (e.type == SDL_MOUSEMOTION){
 					cposX = e.motion.x;
 					cposY = e.motion.y;
				}
 				if ( e.type == SDL_KEYDOWN )
					if ( e.key.keysym.sym == SDLK_SPACE ){
						for( int i=0; i<cell_spawn; i++)
							for( int j=0; j<cell_spawn; j++)
								if( !part_matrix[cposY + i][cposX + j] )
									wood.emplace_back( cposX+j, cposY+i, CELL_WOOD );
					} else if ( e.key.keysym.sym == SDLK_a ){
						for (int i = 0; i<cell_spawn; i++){
							particulas.emplace_back( rand()%( (cposX - 40/2) +40 - (cposX - 40/2))+ (cposX - 40/2),
													 (cposY - 40/2) + 40,
													 CELL_SAND );
						}
					} else if ( e.key.keysym.sym == SDLK_s ){
						for (int i = 0; i<cell_spawn; i++){
							particulas.emplace_back( rand()%( (cposX - 40/2) +40 - (cposX - 40/2))+ (cposX - 40/2),
													 (cposY - 40/2) ,
													 CELL_SMOKE );
						}
					}
			 }
			 
			 //En cada frame, hacemos el color de dibujado en blanco y limpiamos la pantalla
			 SDL_SetRenderDrawColor( win_renderer, 0, 0, 0, 255);
			 SDL_RenderClear(win_renderer);
			 
			 //Ahora dibujamos cosas....
			 SDL_Rect red_rect = {cposX - 40/2, cposY - 40/2, 40,40};
			 SDL_SetRenderDrawColor( win_renderer, 255, 0, 0, 255);
			 SDL_RenderFillRect( win_renderer, &red_rect);
			 
			 //Creamos peque�as particulas de Sand
			 //Si salen de la pantalla se eliminan
			 //Solo se crean 250 maximo
			 
			 particulas.shrink_to_fit();
			 //if ( particulas.size() < 4000 )
				//particulas.emplace_back( rand()%(red_rect.x+40-red_rect.x)+red_rect.x, red_rect.y+40, rand()%2 );
			 
			 for (int i=0; i<480; i++)
			 	for (int j=0; j<640; j++)
			 		part_matrix[i][j] = 0;
			 
			  for (int i=0; i < wood.size(); i++){
				
				part_matrix[ (int)wood[i].y ][ (int)wood[i].x ] = true;
				
			 	if( wood.size() >= wood_max )
					wood.erase( wood.begin()+i );
					
			 	if (wood[i].type == CELL_WOOD)
			 		SDL_SetRenderDrawColor( win_renderer, 164, 116, 73, 255);
			 	SDL_RenderDrawPoint( win_renderer, wood[i].x, wood[i].y );
			 }
			 
			 for (int i=0; i < particulas.size(); i++){
				
				particulas[i].rules();
				
				part_matrix[ (int)particulas[i].y ][ (int)particulas[i].x ] = true;
				
			 	if( particulas.size() >= parts_max )
					particulas.erase( particulas.begin()+i );
					
				if (particulas[i].type == CELL_SAND)
			 		SDL_SetRenderDrawColor( win_renderer, 255, 255, 0, 255);
			 	else if (particulas[i].type == CELL_SMOKE)
			 		SDL_SetRenderDrawColor( win_renderer, 200, 200, 200, 100);
			 	else if (particulas[i].type == CELL_WOOD)
			 		SDL_SetRenderDrawColor( win_renderer, 164, 116, 73, 255);
			 	SDL_RenderDrawPoint( win_renderer, particulas[i].x, particulas[i].y );
			 }
			 
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
        window = SDL_CreateWindow(  "Tutorial SDL: Primitivas geometricas",
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
            // win_surface = SDL_GetWindowSurface(window);
            //Y creamos un renderer acelerado de la ventana principal con CreateRenderer
            win_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
            if (win_renderer == NULL){
        		cerr<<"No se ha podido crear el renderer: "<<SDL_GetError()<<endl;
				success = false;
			}
    }
    return success;	
}

bool load(){

    //Succes flag
    bool success = true;

	//Para este tutorial no cargamos nada
    return success;
}

// SDL_Surface* load_surface( const char* path ){
// 	SDL_Surface* optimized_surface = NULL; //Superficie optimizada para blitting
// 	SDL_Surface* png_surface = IMG_Load(path); //Imagen cargada mediante SDL_Image
// 	if( png_surface == NULL){
// 		cerr<<"No se ha podido cargar la im�gen en "<<path<<":"<<IMG_GetError()<<endl;
// 	}else{
// 		//Optimizamos la superficie y regresamos la imagen
// 		optimized_surface = SDL_ConvertSurface( png_surface, win_surface->format, 0);
// 		if( optimized_surface == NULL ){
// 			cerr<<"No se ha posido optimizar la superficie: "<<SDL_GetError()<<endl;
// 		}else{
// 			SDL_FreeSurface(png_surface);
// 			return optimized_surface;
// 		}
// 	}
// 	return NULL;
// }

void exit(){

    //Liberamos las superficies globales utilizadas y destruimos las ventanas
    //Ademas de colocar los punteros usados a nulo
    SDL_FreeSurface(img_surface);
    img_surface = NULL;
    
    // SDL_FreeSurface(win_surface);
    // win_surface = NULL;

	SDL_DestroyRenderer(win_renderer);
	win_renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

}
