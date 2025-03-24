#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

# define WIDTH 130
# define HEIGHT 28
# define SPEED 0.8

int cube_width = 10;
float alpha = 0, beta = 0, teta = 0;
float i, j, k;
char buffer[WIDTH * HEIGHT];
float depth[WIDTH * HEIGHT];
char wallpaper = ' ';
int center_x = WIDTH/2;
int center_y = HEIGHT/2;
int distance_user = 40;
int scaling_factor = 40;

void matriz_rotacao(float* i, float* j, float* k, float x, float y, float z);

int main(void)
{
	do
	{
		// preencher buffer todo com espacos para o plano de fundo
		memset(buffer, wallpaper, WIDTH * HEIGHT);
		// resetar o array que guarda as profundidades
		memset(depth, 0, sizeof(depth));
		
		for (float x = 0; x < cube_width; x += SPEED)
		{
			for(float y = 0; y < cube_width; y += SPEED)
			{
				matriz_rotacao(&i, &j, &k, x, y, cube_width);
				// ajustar profundidade
				k += distance_user;
				float depth_corrector = (1 / k);
				// mapeando coordenadas 3d para 2d ( objetos mais distantes parecem menores, novas coordenadas vao ser inversamente proporcionais a *k)
				int x_2d = center_x + (j * depth_corrector) * scaling_factor;
				int y_2d = center_y + (i * depth_corrector) * scaling_factor;
				int index = x_2d + y_2d * WIDTH;
				if (index >= 0 && index < WIDTH * HEIGHT) 
				{
        			if (depth_corrector > depth[index])
					{
            			depth[index] = depth_corrector;
            			buffer[index] = '#';
       				}
				}
			}
		}
		// mover cursor para o canto esquerdo da tela para limpar a tela
		printf("\x1b[H");
		for (int i = 0; i < WIDTH * HEIGHT; i++) 
		{
		    putchar(buffer[i]);
		}
		
		alpha += 0.05;
		beta += 0.05;
		teta += 0.05;
		
		// Adicionar delay entre os frames
		usleep(50);

	}while(1);
}

/*  para rotacionar o plano cartesiano tridimensional em torno dos eixos z, y e x respectivamente:
      ( cos sen 0 )
 rz = (-sen cos 0 )(xy)
      ( 0   0  1 )
	
      ( cos 0 sen )
 ry = (  0  1  0  )(xz)
      (-sen 0 cos )
   
       ( 1   0   0 )
 rx =  ( 0 cos sen )(yz)
       ( 0 -sen cos) */
void matriz_rotacao(float* i, float* j, float* k, float x, float y, float z)
{
	// combinando todas as rotacoes : (i' j' k') = (i j k)(rx * ry * rz)
	*i = y * sin(alpha) * sin(beta) * cos(teta) - z * cos(alpha) * sin(beta) * cos(teta) + y * cos(alpha) * sin(teta) + z * sin(alpha) * sin(teta) + x * cos(beta) * cos (teta);
	*j = y * cos(alpha) * cos(teta) + z * sin(alpha) * cos(teta) - y * sin(alpha) * sin(beta) * sin(teta) + z * cos(alpha) * sin(beta) * sin(teta) - x * cos(beta) * sin (teta);
	*k = z * cos(alpha) * cos(beta) - y * sin(alpha) * cos(beta) + x * sin(beta);
}



