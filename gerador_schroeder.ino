#define _TASK_MICRO_RES
#include <TaskScheduler.h>
#include <math.h>
#define PI 3.1415926535897932384626433832795

////////////////////////////////////////
#define Np 5000                       // Numero de pontos
double dt = (double)1 / 20000;
int    ff = 7900;                     // Frequencia final
double T  = (double)Np * dt;          // Amostragem;
double  A = 0.0025;                   // Amplitude
double df = (double)1 / (Np*dt);      // Decalagem da frequencia
double Nh = (double)(ff / df);        // Numero de harmonicas
double fi = 0.0;
int i = 0;
double saida[Np];                     // Saida sem converter o sinal
byte Sinal[Np];                       // Saida convertida para o DAC
////////////////////////////////////////

Scheduler runner;
void Gerador();
void Schroeder(void);
Task TarefaGerador(50, TASK_FOREVER, &Gerador);

void setup()
{
  Schroeder();
  runner.addTask(TarefaGerador);
  TarefaGerador.enable();
}

void loop()
{
  runner.execute();
}

void Gerador()
{
  dacWrite(25, Sinal[i]);
  i++;
  if (i >= Np) i = 0;
}

void Schroeder(void)
{
  double t = 0.0;
  double maximo = 0.0;
  double minimo = 100.0;

  for (int k = 0; k < Nh; k++)    // Calcula a soma de cossenos
  {
    fi = (double) - k * (k - 1) * PI / Nh;
    for (int m = 0; m < Np; m++)
    {
      saida[m] = (double)saida[m] + (double)A * cos((double)(2 * PI * (k + 1) * df * t) + fi);
      if (saida[m] > maximo) maximo = saida[m];
      if (saida[m] < minimo) minimo = saida[m];
      t = (double)t + dt;
    }
    t = 0.0;
  }

  for (int n = 0; n < Np; n++) Sinal[n] = 255 * ((saida[n] - minimo) / (maximo - minimo));
}
