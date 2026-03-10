# Push_Swap: Tutorial Code-Along en Espanol

> **Construye push_swap desde cero, paso a paso.**
> Este tutorial asume que ya tienes `libft` compilada con `ft_printf` y `get_next_line`.

---

## Indice

| Capitulo | Tema                                                                                     | Archivos                                                   |
| -------- | ---------------------------------------------------------------------------------------- | ---------------------------------------------------------- |
| 0        | [Introduccion](#capitulo-0-introduccion)                                                 | -                                                          |
| 1        | [Estructura del Proyecto y Makefile](#capitulo-1-estructura-del-proyecto-y-makefile)     | `push_swap.h`, `Makefile`                                  |
| 2        | [Las Pilas - Inicializacion y Memoria](#capitulo-2-las-pilas---inicializacion-y-memoria) | `src/stack_init.c`                                         |
| 3        | [Las 11 Operaciones](#capitulo-3-las-11-operaciones)                                     | `src/stack_ops_*.c`                                        |
| 4        | [Wrappers de Salida y Tracking](#capitulo-4-wrappers-de-salida-y-tracking)               | `src/ops_out_*.c`                                          |
| 5        | [Parsing de Argumentos](#capitulo-5-parsing-de-argumentos)                               | `src/parse_utils.c`, `src/parse_args.c`                    |
| 6        | [El Main - Punto de Entrada](#capitulo-6-el-main---punto-de-entrada)                     | `src/main.c`                                               |
| 7        | [Utilidades de Ordenamiento](#capitulo-7-utilidades-de-ordenamiento)                     | `src/utils.c`                                              |
| 8        | [Normalizacion](#capitulo-8-normalizacion)                                               | `src/normalize.c`                                          |
| 9        | [Metrica de Desorden](#capitulo-9-metrica-de-desorden)                                   | `src/disorder.c`                                           |
| 10       | [Algoritmo Tiny (2, 3, 5 elementos)](#capitulo-10-algoritmo-tiny-2-3-y-5-elementos)      | `src/sort_tiny.c`                                          |
| 11       | [Algoritmo Simple - Selection Sort](#capitulo-11-algoritmo-simple---selection-sort-on2)  | `src/sort_simple.c`                                        |
| 12       | [Algoritmo Medium - Chunk Sort](#capitulo-12-algoritmo-medium---chunk-sort-onsqrtn)      | `src/sort_medium.c`, `src/sort_medium_utils.c`             |
| 13       | [Algoritmo Complex - Radix Sort](#capitulo-13-algoritmo-complex---radix-sort-onlogn)     | `src/sort_complex.c`                                       |
| 14       | [Algoritmo Adaptativo](#capitulo-14-algoritmo-adaptativo)                                | `src/sort_adaptive.c`                                      |
| 15       | [Benchmark](#capitulo-15-benchmark)                                                      | `src/bench.c`                                              |
| 16       | [Bonus: El Checker](#capitulo-16-bonus-el-checker)                                       | `bonus/checker_main_bonus.c`, `bonus/checker_exec_bonus.c` |
| 17       | [Pruebas y Optimizacion](#capitulo-17-pruebas-y-optimizacion)                            | -                                                          |

---

## Capitulo 0: Introduccion

### Que es push_swap?

`push_swap` es un proyecto de 42 donde tienes que ordenar una lista de numeros
enteros usando **dos pilas** (A y B) y un conjunto limitado de **11 operaciones**.
El objetivo es ordenar los numeros en la pila A usando el **menor numero de
operaciones posible**.

### Las reglas

- Recibes numeros enteros como argumentos del programa.
- Todos los numeros van inicialmente a la **pila A**. La **pila B** empieza vacia.
- Solo puedes usar las 11 operaciones definidas abajo.
- Al final, la pila A debe estar ordenada de menor a mayor, y la pila B vacia.
- El programa imprime cada operacion usada, una por linea.

### Las 11 operaciones

```
sa  : swap a      - Intercambia los 2 primeros elementos de A.
sb  : swap b      - Intercambia los 2 primeros elementos de B.
ss  : swap both   - Hace sa y sb al mismo tiempo.
pa  : push a      - Toma el primer elemento de B y lo pone encima de A.
pb  : push b      - Toma el primer elemento de A y lo pone encima de B.
ra  : rotate a    - Desplaza todos los elementos de A hacia arriba. El primero pasa al final.
rb  : rotate b    - Desplaza todos los elementos de B hacia arriba. El primero pasa al final.
rr  : rotate both - Hace ra y rb al mismo tiempo.
rra : reverse rotate a - Desplaza todos los elementos de A hacia abajo. El ultimo pasa al inicio.
rrb : reverse rotate b - Desplaza todos los elementos de B hacia abajo. El ultimo pasa al inicio.
rrr : reverse rotate both - Hace rra y rrb al mismo tiempo.
```

### Diagramas visuales

**sa / sb (swap):** Intercambia los 2 primeros.

```
Antes:    Despues:
  [3]       [5]
  [5]  -->  [3]
  [1]       [1]
  [4]       [4]
```

**pa (push a):** Mueve el tope de B al tope de A.

```
  A     B        A     B
 [2]   [7]      [7]   [1]
 [5]   [1]  --> [2]
 [3]             [5]
                 [3]
```

**pb (push b):** Mueve el tope de A al tope de B.

```
  A     B        A     B
 [7]   [1]      [2]   [7]
 [2]         --> [5]   [1]
 [5]             [3]
 [3]
```

**ra / rb (rotate):** El primero pasa al final.

```
Antes:    Despues:
  [3]       [5]
  [5]  -->  [1]
  [1]       [4]
  [4]       [3]   <-- el 3 bajo al fondo
```

**rra / rrb (reverse rotate):** El ultimo pasa al inicio.

```
Antes:    Despues:
  [3]       [4]   <-- el 4 subio al tope
  [5]  -->  [3]
  [1]       [5]
  [4]       [1]
```

---

## Capitulo 1: Estructura del Proyecto y Makefile

### Arbol de directorios

Antes de escribir codigo, crea la estructura:

```
pushswap/
  |-- includes/
  |     |-- push_swap.h
  |-- src/
  |     |-- (aqui iran todos los .c)
  |-- bonus/
  |     |-- (aqui ira el checker)
  |-- libft/
  |     |-- libft.a  (ya compilada)
  |     |-- libft.h
  |     |-- ft_printf.h
  |     |-- get_next_line.h
  |-- Makefile
```

```bash
mkdir -p includes src bonus
```

### El header: `includes/push_swap.h`

Este archivo define **todas** las estructuras y prototipos del proyecto. Es el
mapa completo de lo que vamos a construir.

Crea el archivo `includes/push_swap.h` con el siguiente contenido:

```c
#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "../libft/libft.h"
# include "../libft/ft_printf.h"
# include <limits.h>

/* ------ Codigos de operacion (para tracking) ------ */
# define OP_SA   0
# define OP_SB   1
# define OP_SS   2
# define OP_PA   3
# define OP_PB   4
# define OP_RA   5
# define OP_RB   6
# define OP_RR   7
# define OP_RRA  8
# define OP_RRB  9
# define OP_RRR  10

/* ------ Estrategias de ordenamiento ------ */
# define STRAT_SIMPLE   0
# define STRAT_MEDIUM   1
# define STRAT_COMPLEX  2
# define STRAT_ADAPTIVE 3

/* ------ Estructura de pila (array dinamico) ------ */
typedef struct s_stack
{
 int  *data;      // Array de enteros
 int  size;       // Cuantos elementos hay actualmente
 int  capacity;   // Cuantos caben antes de necesitar realloc
} t_stack;

/* ------ Contexto global del programa ------ */
typedef struct s_context
{
 t_stack a;          // Pila A
 t_stack b;          // Pila B
 int  ops[11];    // Contador por cada tipo de operacion
 int  total_ops;  // Total de operaciones ejecutadas
 int  strategy;   // Estrategia elegida (STRAT_*)
 int  bench;      // Flag: mostrar benchmark al final?
 double disorder;   // Metrica de desorden (0.0 a 1.0)
} t_context;

/* stack_init.c */
int  stack_create(t_stack *s, int capacity);
void stack_destroy(t_stack *s);
void free_context(t_context *ctx);

/* normalize.c */
void normalize_stack(t_stack *a);

/* stack_ops_swap.c */
void op_sa(t_stack *a);
void op_sb(t_stack *b);
void op_ss(t_stack *a, t_stack *b);

/* stack_ops_push.c */
void op_pa(t_stack *a, t_stack *b);
void op_pb(t_stack *a, t_stack *b);

/* stack_ops_rotate.c */
void op_ra(t_stack *a);
void op_rb(t_stack *b);
void op_rr(t_stack *a, t_stack *b);

/* stack_ops_revrot.c */
void op_rra(t_stack *a);
void op_rrb(t_stack *b);
void op_rrr(t_stack *a, t_stack *b);

/* ops_out_swap.c */
void do_sa(t_context *ctx);
void do_sb(t_context *ctx);
void do_ss(t_context *ctx);
void do_pa(t_context *ctx);
void do_pb(t_context *ctx);

/* ops_out_rot.c */
void do_ra(t_context *ctx);
void do_rb(t_context *ctx);
void do_rr(t_context *ctx);

/* ops_out_revrot.c */
void do_rra(t_context *ctx);
void do_rrb(t_context *ctx);
void do_rrr(t_context *ctx);

/* parse_utils.c */
int  ft_atol_safe(const char *str, long *result);
int  is_valid_integer(const char *str);

/* parse_args.c */
int  parse_args(int argc, char **argv, t_context *ctx);

/* disorder.c */
double compute_disorder(t_stack *a);

/* utils.c */
int  is_sorted(t_stack *a);
int  find_min_pos(t_stack *s);
int  find_max_pos(t_stack *s);
void rotate_to_top(t_context *ctx, int pos, int stack_id);

/* sort_tiny.c */
void sort_two(t_context *ctx);
void sort_three(t_context *ctx);
void sort_five(t_context *ctx);

/* sort_simple.c */
void sort_simple(t_context *ctx);

/* sort_medium.c */
void sort_medium(t_context *ctx);

/* sort_complex.c */
void sort_complex(t_context *ctx);

/* sort_adaptive.c */
void sort_adaptive(t_context *ctx);

/* bench.c */
void print_bench(t_context *ctx);

/* main.c */
void error_exit(t_context *ctx);

#endif
```

**Que acabamos de hacer?**

- `t_stack`: Nuestra pila es un array dinamico. `data` apunta a la memoria,
  `size` nos dice cuantos elementos tiene, `capacity` cuantos caben.
- `t_context`: Agrupa todo el estado del programa: ambas pilas, contadores
  de operaciones, la estrategia de sort, y la metrica de desorden.
- Los `#define` nos dan nombres legibles para los indices de operaciones y
  las estrategias.

### El Makefile

Crea el archivo `Makefile` en la raiz del proyecto:

```makefile
NAME = push_swap
BONUS_NAME = checker
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I includes -I libft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = src/main.c \
    src/parse_args.c \
    src/parse_utils.c \
    src/stack_init.c \
    src/stack_ops_swap.c \
    src/stack_ops_push.c \
    src/stack_ops_rotate.c \
    src/stack_ops_revrot.c \
    src/ops_out_swap.c \
    src/ops_out_rot.c \
    src/ops_out_revrot.c \
    src/normalize.c \
    src/disorder.c \
    src/sort_tiny.c \
    src/sort_simple.c \
    src/sort_medium.c \
    src/sort_medium_utils.c \
    src/sort_complex.c \
    src/sort_adaptive.c \
    src/bench.c \
    src/utils.c

BONUS_SRCS = bonus/checker_main_bonus.c \
    bonus/checker_exec_bonus.c \
    src/parse_args.c \
    src/parse_utils.c \
    src/stack_init.c \
    src/stack_ops_swap.c \
    src/stack_ops_push.c \
    src/stack_ops_rotate.c \
    src/stack_ops_revrot.c \
    src/ops_out_swap.c \
    src/ops_out_rot.c \
    src/ops_out_revrot.c \
    src/normalize.c \
    src/utils.c

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
 $(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
 $(MAKE) -C $(LIBFT_DIR)

bonus: $(LIBFT) $(BONUS_OBJS)
 $(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(BONUS_NAME)

%.o: %.c
 $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
 $(MAKE) -C $(LIBFT_DIR) clean
 rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
 $(MAKE) -C $(LIBFT_DIR) fclean
 rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus
```

**Desglose del Makefile:**

- `$(NAME)` depende de `$(LIBFT)` (la libft compilada) y de todos los `.o`.
- La regla `%.o: %.c` compila cada `.c` en su `.o` correspondiente.
- `bonus` compila el checker reutilizando archivos de `src/` que son comunes.
- `INCLUDES` le dice al compilador donde buscar los `.h`.

> **Nota:** Todavia no podemos compilar porque no existen los `.c`. Eso lo
> arreglamos a partir del siguiente capitulo.

---

## Capitulo 2: Las Pilas - Inicializacion y Memoria

### Por que arrays en vez de listas enlazadas?

En push_swap necesitamos acceso por indice constantemente (para buscar el
minimo, normalizar, hacer radix por bits, etc.). Un array nos da O(1) para
acceso por indice. Las listas enlazadas serian O(n) para lo mismo.

La desventaja del array es que `push`/`pop` al frente son O(n) porque hay
que desplazar elementos. Pero como nuestras pilas nunca van a tener mas de
unos pocos miles de elementos, esta bien.

### Crea el archivo `src/stack_init.c`

```c
#include "push_swap.h"

/*
** stack_create: Reserva memoria para la pila.
** Recibe un puntero a t_stack y la capacidad deseada.
** Retorna 1 si ok, 0 si malloc falla.
*/
int stack_create(t_stack *s, int capacity)
{
 s->data = malloc(sizeof(int) * capacity);
 if (!s->data)
  return (0);
 s->size = 0;
 s->capacity = capacity;
 return (1);
}

/*
** stack_destroy: Libera la memoria de la pila y resetea los campos.
*/
void stack_destroy(t_stack *s)
{
 if (s->data)
  free(s->data);
 s->data = NULL;
 s->size = 0;
 s->capacity = 0;
}

/*
** free_context: Libera ambas pilas del contexto.
** Llamamos esto antes de salir del programa.
*/
void free_context(t_context *ctx)
{
 stack_destroy(&ctx->a);
 stack_destroy(&ctx->b);
}
```

**Linea por linea:**

1. `stack_create` reserva un bloque de `capacity * sizeof(int)` bytes.
   Si `malloc` falla, retorna 0.
2. `stack_destroy` libera la memoria y pone todo a 0/NULL para evitar
   double-free o uso despues de free.
3. `free_context` es un atajo: libera ambas pilas de una vez.

> **Verifica:** Todavia no compila solo, pero puedes verificar la sintaxis con:
>
> ```bash
> cc -Wall -Wextra -Werror -I includes -I libft -c src/stack_init.c -o src/stack_init.o
> ```

---

## Capitulo 3: Las 11 Operaciones

Aqui implementamos la **logica pura** de cada operacion. Estas funciones
modifican las pilas pero **no imprimen nada**. La impresion la manejamos
en el capitulo 4.

### 3.1 Swap: `src/stack_ops_swap.c`

**Concepto:** Intercambiar los dos primeros elementos de la pila.

```
Antes sa:  [3][5][1][4]
Despues:   [5][3][1][4]
```

Crea `src/stack_ops_swap.c`:

```c
#include "push_swap.h"

/*
** op_sa: Intercambia los 2 primeros elementos de la pila A.
** Si hay menos de 2 elementos, no hace nada.
*/
void op_sa(t_stack *a)
{
 int tmp;

 if (a->size < 2)
  return ;
 tmp = a->data[0];
 a->data[0] = a->data[1];
 a->data[1] = tmp;
}

/*
** op_sb: Igual que sa pero para la pila B.
*/
void op_sb(t_stack *b)
{
 int tmp;

 if (b->size < 2)
  return ;
 tmp = b->data[0];
 b->data[0] = b->data[1];
 b->data[1] = tmp;
}

/*
** op_ss: Hace sa y sb al mismo tiempo.
*/
void op_ss(t_stack *a, t_stack *b)
{
 op_sa(a);
 op_sb(b);
}
```

Es sencillo: guardamos `data[0]` en `tmp`, movemos `data[1]` a `data[0]`,
y ponemos `tmp` en `data[1]`.

### 3.2 Push: `src/stack_ops_push.c`

**Concepto:** Tomar el primer elemento de una pila y ponerlo encima de la otra.

```
Antes pb:   A=[7][2][5]   B=[1]
Despues:    A=[2][5]      B=[7][1]
```

Para esto necesitamos dos helpers: `push_front` (meter al inicio) y
`pop_front` (sacar del inicio).

Crea `src/stack_ops_push.c`:

```c
#include "push_swap.h"

/*
** push_front: Inserta un valor al inicio del array.
** Desplaza todos los elementos una posicion a la derecha.
**
**   Antes:  [5][1][4]  (size=3)
**   push_front(s, 7)
**   Despues: [7][5][1][4]  (size=4)
*/
static void push_front(t_stack *s, int val)
{
 int i;

 i = s->size;
 while (i > 0)
 {
  s->data[i] = s->data[i - 1];
  i--;
 }
 s->data[0] = val;
 s->size++;
}

/*
** pop_front: Saca y retorna el primer elemento.
** Desplaza todos los elementos una posicion a la izquierda.
**
**   Antes:  [7][5][1][4]  (size=4)
**   pop_front(s) -> retorna 7
**   Despues: [5][1][4]  (size=3)
*/
static int pop_front(t_stack *s)
{
 int val;
 int i;

 val = s->data[0];
 i = 0;
 while (i < s->size - 1)
 {
  s->data[i] = s->data[i + 1];
  i++;
 }
 s->size--;
 return (val);
}

/*
** op_pa: push a - Toma el tope de B y lo pone encima de A.
*/
void op_pa(t_stack *a, t_stack *b)
{
 if (b->size == 0)
  return ;
 push_front(a, pop_front(b));
}

/*
** op_pb: push b - Toma el tope de A y lo pone encima de B.
*/
void op_pb(t_stack *a, t_stack *b)
{
 if (a->size == 0)
  return ;
 push_front(b, pop_front(a));
}
```

**Por que `static`?** `push_front` y `pop_front` son helpers internos. Solo
se usan dentro de este archivo, asi que las marcamos `static` para no
contaminar el namespace global (y cumplir las normas de 42).

### 3.3 Rotate: `src/stack_ops_rotate.c`

**Concepto:** El primer elemento baja al fondo. Todos los demas suben una posicion.

```
Antes ra:   [3][5][1][4]
Despues:    [5][1][4][3]
            ^           ^
            todo sube    el 3 bajo al fondo
```

Crea `src/stack_ops_rotate.c`:

```c
#include "push_swap.h"

/*
** rotate: Helper interno. Desplaza todo hacia arriba.
** El primer elemento se va al final.
*/
static void rotate(t_stack *s)
{
 int tmp;
 int i;

 if (s->size < 2)
  return ;
 tmp = s->data[0];        // Guardamos el primero
 i = 0;
 while (i < s->size - 1)
 {
  s->data[i] = s->data[i + 1];  // Cada uno sube una posicion
  i++;
 }
 s->data[s->size - 1] = tmp;  // El primero va al final
}

void op_ra(t_stack *a)
{
 rotate(a);
}

void op_rb(t_stack *b)
{
 rotate(b);
}

void op_rr(t_stack *a, t_stack *b)
{
 rotate(a);
 rotate(b);
}
```

### 3.4 Reverse Rotate: `src/stack_ops_revrot.c`

**Concepto:** El ultimo elemento sube al tope. Todos los demas bajan una posicion.
Es lo opuesto de rotate.

```
Antes rra:  [3][5][1][4]
Despues:    [4][3][5][1]
             ^           ^
             el 4 subio   todo lo demas bajo
```

Crea `src/stack_ops_revrot.c`:

```c
#include "push_swap.h"

/*
** rev_rotate: Helper interno. Desplaza todo hacia abajo.
** El ultimo elemento se va al inicio.
*/
static void rev_rotate(t_stack *s)
{
 int tmp;
 int i;

 if (s->size < 2)
  return ;
 tmp = s->data[s->size - 1];  // Guardamos el ultimo
 i = s->size - 1;
 while (i > 0)
 {
  s->data[i] = s->data[i - 1];  // Cada uno baja una posicion
  i--;
 }
 s->data[0] = tmp;  // El ultimo va al inicio
}

void op_rra(t_stack *a)
{
 rev_rotate(a);
}

void op_rrb(t_stack *b)
{
 rev_rotate(b);
}

void op_rrr(t_stack *a, t_stack *b)
{
 rev_rotate(a);
 rev_rotate(b);
}
```

> **Verifica:** Compila todos los `.o` de operaciones:
>
> ```bash
> cc -Wall -Wextra -Werror -I includes -I libft -c src/stack_ops_swap.c -o src/stack_ops_swap.o
> cc -Wall -Wextra -Werror -I includes -I libft -c src/stack_ops_push.c -o src/stack_ops_push.o
> cc -Wall -Wextra -Werror -I includes -I libft -c src/stack_ops_rotate.c -o src/stack_ops_rotate.o
> cc -Wall -Wextra -Werror -I includes -I libft -c src/stack_ops_revrot.c -o src/stack_ops_revrot.o
> ```
>
> Si no hay errores, vas bien.

---

## Capitulo 4: Wrappers de Salida y Tracking

### Por que separar logica de salida?

Las funciones `op_*` del capitulo anterior son **logica pura**: modifican las
pilas y nada mas. Pero push_swap necesita:

1. Ejecutar la operacion.
2. Imprimir el nombre de la operacion (`"sa\n"`, `"pb\n"`, etc.).
3. Incrementar el contador de operaciones (para benchmark).

Las funciones `do_*` hacen estas 3 cosas. Separar permite que el **checker**
(bonus) reutilice las `op_*` sin imprimir nada.

### 4.1 Swap y Push output: `src/ops_out_swap.c`

Crea `src/ops_out_swap.c`:

```c
#include "push_swap.h"

void do_sa(t_context *ctx)
{
 op_sa(&ctx->a);             // 1. Ejecuta la operacion
 write(1, "sa\n", 3);       // 2. Imprime el nombre
 ctx->ops[OP_SA]++;         // 3. Incrementa contador individual
 ctx->total_ops++;           // 4. Incrementa contador total
}

void do_sb(t_context *ctx)
{
 op_sb(&ctx->b);
 write(1, "sb\n", 3);
 ctx->ops[OP_SB]++;
 ctx->total_ops++;
}

void do_ss(t_context *ctx)
{
 op_ss(&ctx->a, &ctx->b);
 write(1, "ss\n", 3);
 ctx->ops[OP_SS]++;
 ctx->total_ops++;
}

void do_pa(t_context *ctx)
{
 op_pa(&ctx->a, &ctx->b);
 write(1, "pa\n", 3);
 ctx->ops[OP_PA]++;
 ctx->total_ops++;
}

void do_pb(t_context *ctx)
{
 op_pb(&ctx->a, &ctx->b);
 write(1, "pb\n", 3);
 ctx->ops[OP_PB]++;
 ctx->total_ops++;
}
```

**Patron comun:** Todas las funciones `do_*` siguen exactamente el mismo patron:
ejecutar, escribir, contar. Es repetitivo pero claro.

### 4.2 Rotate output: `src/ops_out_rot.c`

Crea `src/ops_out_rot.c`:

```c
#include "push_swap.h"

void do_ra(t_context *ctx)
{
 op_ra(&ctx->a);
 write(1, "ra\n", 3);
 ctx->ops[OP_RA]++;
 ctx->total_ops++;
}

void do_rb(t_context *ctx)
{
 op_rb(&ctx->b);
 write(1, "rb\n", 3);
 ctx->ops[OP_RB]++;
 ctx->total_ops++;
}

void do_rr(t_context *ctx)
{
 op_rr(&ctx->a, &ctx->b);
 write(1, "rr\n", 3);
 ctx->ops[OP_RR]++;
 ctx->total_ops++;
}
```

### 4.3 Reverse Rotate output: `src/ops_out_revrot.c`

Crea `src/ops_out_revrot.c`:

```c
#include "push_swap.h"

void do_rra(t_context *ctx)
{
 op_rra(&ctx->a);
 write(1, "rra\n", 4);      // Nota: "rra\n" son 4 bytes, no 3
 ctx->ops[OP_RRA]++;
 ctx->total_ops++;
}

void do_rrb(t_context *ctx)
{
 op_rrb(&ctx->b);
 write(1, "rrb\n", 4);
 ctx->ops[OP_RRB]++;
 ctx->total_ops++;
}

void do_rrr(t_context *ctx)
{
 op_rrr(&ctx->a, &ctx->b);
 write(1, "rrr\n", 4);
 ctx->ops[OP_RRR]++;
 ctx->total_ops++;
}
```

> **Ojo con el tamanio:** `"sa\n"` = 3 bytes, pero `"rra\n"` = 4 bytes.
> Si pones el numero equivocado en `write`, vas a tener bugs silenciosos.

---

## Capitulo 5: Parsing de Argumentos

El parsing es una de las partes mas delicadas. Hay que manejar:

- Numeros negativos y positivos.
- Argumentos individuales: `./push_swap 3 2 1`
- Un solo string con espacios: `./push_swap "3 2 1"`
- Mezcla de ambos: `./push_swap 3 "2 1"`
- Overflow de `int` (numeros fuera del rango INT_MIN a INT_MAX).
- Duplicados.
- Argumentos no numericos.

Si **cualquier** cosa esta mal, el programa imprime `Error\n` a stderr y sale.

### 5.1 Utilidades de parsing: `src/parse_utils.c`

Crea `src/parse_utils.c`:

```c
#include "push_swap.h"

/*
** is_valid_integer: Verifica que el string tenga formato de entero valido.
** Acepta un signo opcional (+/-) seguido de uno o mas digitos.
** Retorna 1 si valido, 0 si no.
**
** Ejemplos:
**   "42"  -> 1 (valido)
**   "-42" -> 1 (valido)
**   "+42" -> 1 (valido)
**   ""    -> 0 (vacio)
**   "-"   -> 0 (signo sin digitos)
**   "4a2" -> 0 (caracter no numerico)
*/
int is_valid_integer(const char *str)
{
 int i;

 i = 0;
 if (str[i] == '-' || str[i] == '+')
  i++;
 if (!str[i])           // Si despues del signo no hay nada, invalido
  return (0);
 while (str[i])
 {
  if (!ft_isdigit(str[i]))  // Si hay un caracter no-digito, invalido
   return (0);
  i++;
 }
 return (1);
}

/*
** ft_atol_safe: Convierte string a long, verificando overflow de int.
** Retorna 1 si la conversion fue exitosa, 0 si hay overflow.
** El resultado se almacena en *result.
**
** Por que long y no int? Porque necesitamos detectar si el numero
** excede INT_MAX o INT_MIN. Si usaramos int, el overflow seria
** comportamiento indefinido en C.
*/
int ft_atol_safe(const char *str, long *result)
{
 int  i;
 int  sign;
 long val;

 i = 0;
 sign = 1;
 val = 0;
 if (str[i] == '-' || str[i] == '+')
 {
  if (str[i] == '-')
   sign = -1;
  i++;
 }
 while (str[i])
 {
  val = val * 10 + (str[i] - '0');
  if ((sign == 1 && val > INT_MAX)
   || (sign == -1 && val > (long)INT_MAX + 1))
   return (0);  // Overflow: el numero no cabe en un int
  i++;
 }
 *result = val * sign;
 return (1);
}
```

**Punto clave:** Usamos `long` para acumular el valor. Esto nos permite detectar
si excede `INT_MAX` (2147483647) o `INT_MIN` (-2147483648) **antes** de que
ocurra overflow indefinido.

### 5.2 Parser principal: `src/parse_args.c`

Crea `src/parse_args.c`:

```c
#include "push_swap.h"

/*
** check_duplicates: Verifica que no haya numeros repetidos en la pila.
** Comparacion O(n^2) - para n <= 10000 es suficiente.
*/
static int check_duplicates(t_stack *a)
{
 int i;
 int j;

 i = 0;
 while (i < a->size)
 {
  j = i + 1;
  while (j < a->size)
  {
   if (a->data[i] == a->data[j])
    return (0);  // Duplicado encontrado
   j++;
  }
  i++;
 }
 return (1);  // Sin duplicados
}

/*
** parse_flag: Maneja flags como --simple, --medium, --bench, etc.
** Estos flags NO son parte del subject de 42, son extras nuestros
** para elegir la estrategia de sorting y activar benchmark.
**
** Retorna 0 siempre (no es un error si el flag no se reconoce,
** simplemente se ignora).
*/
static int parse_flag(char *arg, t_context *ctx)
{
 if (ft_strncmp(arg, "--simple", 9) == 0)
  ctx->strategy = STRAT_SIMPLE;
 else if (ft_strncmp(arg, "--medium", 9) == 0)
  ctx->strategy = STRAT_MEDIUM;
 else if (ft_strncmp(arg, "--complex", 10) == 0)
  ctx->strategy = STRAT_COMPLEX;
 else if (ft_strncmp(arg, "--adaptive", 11) == 0)
  ctx->strategy = STRAT_ADAPTIVE;
 else if (ft_strncmp(arg, "--bench", 8) == 0)
  ctx->bench = 1;
 else
    return (0);
 return (1);
}

/*
** add_number: Valida un token y lo anade a la pila A.
** 1. Verifica formato valido (is_valid_integer)
** 2. Verifica que no haya overflow (ft_atol_safe)
** 3. Lo mete al final del array de la pila A
*/
static int add_number(t_context *ctx, char *str)
{
 long val;

 if (!is_valid_integer(str))
  return (0);
 if (!ft_atol_safe(str, &val))
  return (0);
 ctx->a.data[ctx->a.size] = (int)val;
 ctx->a.size++;
 return (1);
}

/*
** parse_split_arg: Maneja un argumento que puede contener espacios.
** Ejemplo: "./push_swap "3 2 1"" -> argv[1] = "3 2 1"
** Usamos ft_split para separar por espacios y procesar cada token.
*/
static int parse_split_arg(t_context *ctx, char *arg)
{
 char **tokens;
 int  i;
 int  ret;

 tokens = ft_split(arg, ' ');
 if (!tokens)
  return (0);
 i = 0;
 ret = 1;
 while (tokens[i] && ret)
 {
  if (tokens[i][0] == '-' && tokens[i][1] == '-')
   ret = parse_flag(tokens[i], ctx);
  else
   ret = add_number(ctx, tokens[i]);
  i++;
 }
 i = 0;
 while (tokens[i])
  free(tokens[i++]);  // Liberamos cada token
 free(tokens);            // Liberamos el array de punteros
 return (ret);
}

/*
** parse_args: Funcion principal de parsing.
** Recorre argv[1..argc-1]:
**   - Si empieza con "--", es un flag.
**   - Si no, lo pasa a parse_split_arg (que maneja tanto
**     numeros sueltos como strings con espacios).
** Al final, verifica que no haya duplicados.
*/
int parse_args(int argc, char **argv, t_context *ctx)
{
 int i;

 i = 1;
 while (i < argc)
 {
  if (argv[i][0] == '-' && argv[i][1] == '-')
  {
   if (!parse_flag(argv[i], ctx))
    return (0);
  }
  else if (!parse_split_arg(ctx, argv[i]))
   return (0);
  i++;
 }
 if (!check_duplicates(&ctx->a))
  return (0);
 return (1);
}
```

**Flujo del parsing:**

```
./push_swap 5 "3 2" --bench 1

argv[0] = "./push_swap"  (se ignora, i empieza en 1)
argv[1] = "5"            -> parse_split_arg -> add_number("5") -> A=[5]
argv[2] = "3 2"          -> parse_split_arg -> ft_split -> add_number("3"), add_number("2") -> A=[5,3,2]
argv[3] = "--bench"      -> parse_flag -> ctx->bench = 1
argv[4] = "1"            -> parse_split_arg -> add_number("1") -> A=[5,3,2,1]

Finalmente: check_duplicates -> OK, no hay repetidos.
```

> **Verifica:** Compila los archivos de parsing:
>
> ```bash
> cc -Wall -Wextra -Werror -I includes -I libft -c src/parse_utils.c -o src/parse_utils.o
> cc -Wall -Wextra -Werror -I includes -I libft -c src/parse_args.c -o src/parse_args.o
> ```

---

## Capitulo 6: El Main - Punto de Entrada

Ahora juntamos todo. El `main` es el director de orquesta.

### Crea `src/main.c`

```c
#include "push_swap.h"

/*
** error_exit: Imprime "Error\n" a stderr y sale con codigo 1.
** Siempre libera la memoria antes de salir.
*/
void error_exit(t_context *ctx)
{
 free_context(ctx);
 write(2, "Error\n", 6);
 exit(1);
}

/*
** init_context: Inicializa todos los campos del contexto a sus valores
** por defecto. Importante hacerlo antes de cualquier otra cosa.
*/
static void init_context(t_context *ctx)
{
 int i;

 ctx->total_ops = 0;
 ctx->strategy = STRAT_ADAPTIVE;  // Por defecto: estrategia adaptativa
 ctx->bench = 0;
 ctx->disorder = 0.0;
 i = 0;
 while (i < 11)
  ctx->ops[i++] = 0;
}

/*
** dispatch_sort: Decide que algoritmo usar segun ctx->strategy.
** Si ya esta ordenado o tiene <= 1 elemento, no hace nada.
*/
static void dispatch_sort(t_context *ctx)
{
 if (ctx->a.size <= 1 || is_sorted(&ctx->a))
  return ;
 if (ctx->strategy == STRAT_SIMPLE)
  sort_simple(ctx);
 else if (ctx->strategy == STRAT_MEDIUM)
  sort_medium(ctx);
 else if (ctx->strategy == STRAT_COMPLEX)
  sort_complex(ctx);
 else
  sort_adaptive(ctx);
}

/*
** create_stacks: Reserva memoria para ambas pilas.
** Capacidad de 10000: suficiente para el subject de 42.
*/
static int create_stacks(t_context *ctx)
{
 if (!stack_create(&ctx->a, 10000))
  return (0);
 if (!stack_create(&ctx->b, 10000))
 {
  stack_destroy(&ctx->a);
  return (0);
 }
 return (1);
}

/*
** main: Punto de entrada.
** Flujo:
**   1. Si no hay argumentos, salir silenciosamente.
**   2. Inicializar contexto y crear pilas.
**   3. Parsear argumentos (llena pila A).
**   4. Si pila A vacia (ej: solo flags), salir.
**   5. Normalizar valores (convertir a rango 0..n-1).
**   6. Calcular metrica de desorden.
**   7. Ejecutar el algoritmo de sorting.
**   8. Si --bench activo, mostrar estadisticas.
**   9. Liberar memoria y salir.
*/
int main(int argc, char **argv)
{
 t_context ctx;

 if (argc < 2)
  return (0);
 init_context(&ctx);
 if (!create_stacks(&ctx))
  return (1);
 if (!parse_args(argc, argv, &ctx))
  error_exit(&ctx);
 if (ctx.a.size == 0)
 {
  free_context(&ctx);
  return (0);
 }
 normalize_stack(&ctx.a);
 ctx.disorder = compute_disorder(&ctx.a);
 dispatch_sort(&ctx);
 if (ctx.bench)
  print_bench(&ctx);
 free_context(&ctx);
 return (0);
}
```

**Diagrama del flujo:**

```
              main()
                |
          argc < 2? --si--> return 0
                |
         init_context()
                |
        create_stacks()
                |
     parse_args() --falla--> error_exit()
                |
       a.size == 0? --si--> free + return 0
                |
      normalize_stack()
                |
     compute_disorder()
                |
       dispatch_sort()
                |
         bench? --si--> print_bench()
                |
         free_context()
                |
            return 0
```

> **Primer compilacion!** Todavia necesitamos los archivos de los capitulos
> 7-15 para que `make` funcione. Si quieres probar ahora, puedes crear
> archivos stub vacios para `normalize.c`, `disorder.c`, `utils.c`,
> `sort_tiny.c`, `sort_simple.c`, `sort_medium.c`, `sort_medium_utils.c`,
> `sort_complex.c`, `sort_adaptive.c` y `bench.c` con funciones que no
> hagan nada. O simplemente sigue al siguiente capitulo.

---

## Capitulo 7: Utilidades de Ordenamiento

Estas funciones son herramientas que usan **todos** los algoritmos.

### Crea `src/utils.c`

```c
#include "push_swap.h"

/*
** is_sorted: Verifica si la pila esta ordenada de menor a mayor.
** Retorna 1 si esta ordenada, 0 si no.
*/
int is_sorted(t_stack *a)
{
 int i;

 i = 0;
 while (i < a->size - 1)
 {
  if (a->data[i] > a->data[i + 1])
   return (0);  // Encontramos un par desordenado
  i++;
 }
 return (1);
}

/*
** find_min_pos: Encuentra la POSICION (indice) del elemento minimo.
** No retorna el valor, sino donde esta.
*/
int find_min_pos(t_stack *s)
{
 int i;
 int pos;

 pos = 0;
 i = 1;
 while (i < s->size)
 {
  if (s->data[i] < s->data[pos])
   pos = i;
  i++;
 }
 return (pos);
}

/*
** find_max_pos: Igual pero para el maximo.
*/
int find_max_pos(t_stack *s)
{
 int i;
 int pos;

 pos = 0;
 i = 1;
 while (i < s->size)
 {
  if (s->data[i] > s->data[pos])
   pos = i;
  i++;
 }
 return (pos);
}

/*
** rot_dir: Helper que decide si rotar normal o inverso,
** y en que pila (A=0, B=1).
*/
static void rot_dir(t_context *ctx, int stack_id, int reverse)
{
 if (!reverse && stack_id == 0)
  do_ra(ctx);
 else if (!reverse)
  do_rb(ctx);
 else if (stack_id == 0)
  do_rra(ctx);
 else
  do_rrb(ctx);
}

/*
** rotate_to_top: Lleva el elemento en la posicion `pos` al tope de la pila.
** Optimizacion clave: si `pos` esta en la primera mitad, rotamos hacia
** arriba (ra/rb). Si esta en la segunda mitad, rotamos hacia abajo
** (rra/rrb), que es mas corto.
**
** Ejemplo con pila de 10 elementos:
**   pos=2: ra, ra           (2 operaciones)
**   pos=8: rra, rra         (2 operaciones, mejor que 8 ra's)
*/
void rotate_to_top(t_context *ctx, int pos, int stack_id)
{
 int size;

 if (stack_id == 0)
  size = ctx->a.size;
 else
  size = ctx->b.size;
 if (pos <= size / 2)
 {
  while (pos-- > 0)
   rot_dir(ctx, stack_id, 0);   // Rotacion normal
 }
 else
 {
  while (pos++ < size)
   rot_dir(ctx, stack_id, 1);   // Rotacion inversa (mas corta)
 }
}
```

**`rotate_to_top` es clave para la eficiencia.** Si quieres llevar un
elemento al tope, siempre hay que elegir la direccion mas corta:

```
Pila: [a][b][c][d][e][f][g][h][i][j]   (size=10)

Para llevar 'c' (pos=2) al tope:
  Opcion 1: ra, ra        (2 ops) <-- elegimos esta
  Opcion 2: rra * 8       (8 ops)

Para llevar 'i' (pos=8) al tope:
  Opcion 1: ra * 8        (8 ops)
  Opcion 2: rra, rra      (2 ops) <-- elegimos esta
```

---

## Capitulo 8: Normalizacion

### Por que normalizar?

Los numeros de entrada pueden ser cualquier cosa: `-2147483648`, `42`, `999999`.
El algoritmo radix (capitulo 13) trabaja con **bits** de los valores. Si no
normalizamos, necesitariamos manejar numeros negativos en bits, lo cual
complica todo.

**Normalizar** convierte los valores originales a un rango `0..n-1` preservando
el orden relativo:

```
Original:    [42, -5, 1000, 7]
Ordenado:    [-5, 7, 42, 1000]
              0   1   2    3
Normalizado: [2,  0,  3,   1]
```

Asi, si tienes 500 numeros, los valores normalizados van de 0 a 499.

### Crea `src/normalize.c`

```c
#include "push_swap.h"

/*
** sort_copy: Ordena un array con selection sort O(n^2).
** Solo se usa para ordenar la copia temporal, no las pilas.
*/
static void sort_copy(int *arr, int n)
{
 int i;
 int j;
 int tmp;

 i = 0;
 while (i < n - 1)
 {
  j = i + 1;
  while (j < n)
  {
   if (arr[i] > arr[j])
   {
    tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
   }
   j++;
  }
  i++;
 }
}

/*
** bin_search: Busqueda binaria clasica.
** Busca `val` en `sorted` (array ordenado de tamanio n).
** Retorna el indice donde lo encuentra, o -1 si no esta.
** Como normalizamos una copia del array original, siempre lo va a encontrar.
*/
static int bin_search(int *sorted, int n, int val)
{
 int lo;
 int hi;
 int mid;

 lo = 0;
 hi = n - 1;
 while (lo <= hi)
 {
  mid = (lo + hi) / 2;
  if (sorted[mid] == val)
   return (mid);
  else if (sorted[mid] < val)
   lo = mid + 1;
  else
   hi = mid - 1;
 }
 return (-1);
}

/*
** normalize_stack: Convierte los valores de la pila a rango 0..n-1.
**
** Algoritmo:
**   1. Copiar los datos a un array temporal.
**   2. Ordenar la copia.
**   3. Para cada elemento original, buscar su posicion en la copia
**      ordenada. Esa posicion es su valor normalizado.
**
** Ejemplo:
**   Original: [42, -5, 1000, 7]
**   Copia ordenada: [-5, 7, 42, 1000]
**   42 esta en posicion 2 -> normalizado = 2
**   -5 esta en posicion 0 -> normalizado = 0
**   1000 esta en posicion 3 -> normalizado = 3
**   7 esta en posicion 1 -> normalizado = 1
**   Resultado: [2, 0, 3, 1]
*/
void normalize_stack(t_stack *a)
{
 int *sorted;
 int i;

 sorted = malloc(sizeof(int) * a->size);
 if (!sorted)
  return ;
 i = -1;
 while (++i < a->size)
  sorted[i] = a->data[i];      // Copiar datos
 sort_copy(sorted, a->size);       // Ordenar la copia
 i = -1;
 while (++i < a->size)
  a->data[i] = bin_search(sorted, a->size, a->data[i]);  // Reemplazar
 free(sorted);
}
```

**Complejidad:** O(n^2) por el sort + O(n log n) por las busquedas binarias.
En total O(n^2), pero solo se ejecuta una vez al inicio.

---

## Capitulo 9: Metrica de Desorden

### Que son las inversiones?

Una **inversion** es un par de elementos `(i, j)` donde `i < j` pero
`data[i] > data[j]`. Es decir, estan en el orden equivocado.

```
Array: [3, 1, 2]

Pares posibles: (0,1), (0,2), (1,2)
  (0,1): data[0]=3 > data[1]=1  -> inversion!
  (0,2): data[0]=3 > data[2]=2  -> inversion!
  (1,2): data[1]=1 < data[2]=2  -> no es inversion

Total de inversiones: 2
```

El **maximo** de inversiones posibles para n elementos es `n*(n-1)/2`
(cuando el array esta completamente al reves).

La **metrica de desorden** es:

```
disorder = inversiones_actuales / inversiones_maximas
```

- `disorder = 0.0` -> el array esta ordenado.
- `disorder = 1.0` -> el array esta completamente invertido.
- `disorder = 0.15` -> casi ordenado, solo hay que mover unos pocos.

### Crea `src/disorder.c`

```c
#include "push_swap.h"

/*
** count_inversions: Cuenta el numero de inversiones en la pila.
** Una inversion es un par (i,j) donde i<j pero data[i]>data[j].
** Complejidad: O(n^2).
*/
static long count_inversions(t_stack *a)
{
 long inv;
 int  i;
 int  j;

 inv = 0;
 i = 0;
 while (i < a->size)
 {
  j = i + 1;
  while (j < a->size)
  {
   if (a->data[i] > a->data[j])
    inv++;
   j++;
  }
  i++;
 }
 return (inv);
}

/*
** compute_disorder: Calcula la metrica de desorden (0.0 a 1.0).
** Usamos long para evitar overflow: para n=10000,
** max_inv = 10000 * 9999 / 2 = 49,995,000 (cabe en long).
*/
double compute_disorder(t_stack *a)
{
 long inv;
 long max_inv;

 if (a->size <= 1)
  return (0.0);
 inv = count_inversions(a);
 max_inv = (long)a->size * (a->size - 1) / 2;
 if (max_inv == 0)
  return (0.0);
 return ((double)inv / (double)max_inv);
}
```

**Por que `long`?** Para 10,000 elementos, el maximo de inversiones es
~50 millones. Eso cabe en `long` (y en `int` de 32 bits), pero usamos
`long` por seguridad.

**Para que sirve esta metrica?** El algoritmo adaptativo (capitulo 14) la usa
para decidir que estrategia aplicar. Un array casi ordenado necesita una
estrategia diferente a uno completamente aleatorio.

---

## Capitulo 10: Algoritmo Tiny (2, 3 y 5 elementos)

Para cantidades pequenas no necesitamos algoritmos complejos. Podemos
resolver cada caso con una secuencia fija de operaciones.

### sort_two: 2 elementos

Trivial: si estan desordenados, swap.

```
[2, 1] -> sa -> [1, 2]    (1 operacion)
[1, 2] -> ya esta ordenado  (0 operaciones)
```

### sort_three: 3 elementos

Con 3 elementos hay 6 permutaciones posibles. Para cada una hay una secuencia
optima de **a lo mucho 2 operaciones**:

```
Caso            Operaciones
[1, 2, 3]      (nada - ya ordenado)
[2, 1, 3]      sa
[3, 2, 1]      sa, rra
[3, 1, 2]      ra
[1, 3, 2]      sa, ra
[2, 3, 1]      rra
```

Veamos como mapea el codigo a estos casos usando `a=data[0]`, `b=data[1]`,
`c=data[2]`:

```
[2,1,3]: a>b, b<c, a<c  -> sa
[3,2,1]: a>b, b>c       -> sa, rra
[3,1,2]: a>b, b<c, a>c  -> ra
[1,3,2]: a<b, b>c, a<c  -> sa, ra
[2,3,1]: a<b, b>c, a>c  -> rra
```

### sort_five: 4-5 elementos

La estrategia:

1. Encontrar el minimo en A.
2. Rotarlo al tope.
3. Push a B.
4. Repetir hasta que queden 3 en A.
5. sort_three en A.
6. Push todo de B de vuelta a A.

Como los minimos ya estan en B en orden, al devolverlos quedan al inicio de A.

### Crea `src/sort_tiny.c`

```c
#include "push_swap.h"

void sort_two(t_context *ctx)
{
 if (ctx->a.data[0] > ctx->a.data[1])
  do_sa(ctx);
}

void sort_three(t_context *ctx)
{
 int a;
 int b;
 int c;

 a = ctx->a.data[0];
 b = ctx->a.data[1];
 c = ctx->a.data[2];
 if (a > b && b < c && a < c)        // [2,1,3]
  do_sa(ctx);
 else if (a > b && b > c)             // [3,2,1]
 {
  do_sa(ctx);
  do_rra(ctx);
 }
 else if (a > b && b < c && a > c)    // [3,1,2]
  do_ra(ctx);
 else if (a < b && b > c && a < c)    // [1,3,2]
 {
  do_sa(ctx);
  do_ra(ctx);
 }
 else if (a < b && b > c && a > c)    // [2,3,1]
  do_rra(ctx);
}

/*
** push_min_to_b: Encuentra el minimo en A, lo rota al tope, y lo empuja a B.
*/
static void push_min_to_b(t_context *ctx)
{
 int pos;

 pos = find_min_pos(&ctx->a);
 rotate_to_top(ctx, pos, 0);
 do_pb(ctx);
}

/*
** sort_five: Ordena 4 o 5 elementos.
** 1. Empujar los minimos a B hasta que queden 3 en A.
** 2. Ordenar los 3 que quedan en A.
** 3. Devolver todo de B a A.
*/
void sort_five(t_context *ctx)
{
 while (ctx->a.size > 3)
  push_min_to_b(ctx);
 sort_three(ctx);
 while (ctx->b.size > 0)
  do_pa(ctx);
}
```

> **Verifica:**
>
> ```bash
> make
> ./push_swap 2 1           # Debe imprimir: sa
> ./push_swap 3 2 1         # Debe imprimir: sa  rra
> ./push_swap 1 2 3         # No imprime nada (ya ordenado)
> ./push_swap 5 3 2 4 1     # Debe imprimir una secuencia corta
> ```

---

## Capitulo 11: Algoritmo Simple - Selection Sort O(n^2)

### El concepto

Selection sort adaptado a pilas:

1. Encontrar el minimo en A.
2. Rotarlo al tope de A.
3. Empujarlo a B.
4. Repetir hasta vaciar A.
5. Empujar todo de B de vuelta a A.

Como siempre empujamos el minimo, B queda ordenada de mayor a menor.
Al devolver todo, A queda ordenada de menor a mayor.

```
Inicio:  A=[5,3,1,4,2]  B=[]

Paso 1: min=1 (pos=2), ra, ra, pb  -> A=[4,2,5,3]  B=[1]
Paso 2: min=2 (pos=1), ra, pb      -> A=[5,3,4]    B=[2,1]
Paso 3: min=3 (pos=1), ra, pb      -> A=[4,5]      B=[3,2,1]
Paso 4: min=4 (pos=0), pb          -> A=[5]        B=[4,3,2,1]
Paso 5: min=5 (pos=0), pb          -> A=[]         B=[5,4,3,2,1]

Devolver: pa*5 -> A=[1,2,3,4,5]  B=[]
```

**Complejidad:** O(n^2) operaciones. Para n=100 puede dar ~1000-1500 ops.
Para n=500 puede dar ~60,000+ ops (demasiado para 42).

### Crea `src/sort_simple.c`

```c
#include "push_swap.h"

void sort_simple(t_context *ctx)
{
 int pos;

 if (ctx->a.size <= 1 || is_sorted(&ctx->a))
  return ;
 if (ctx->a.size <= 3)
 {
  if (ctx->a.size == 2)
   sort_two(ctx);
  else
   sort_three(ctx);
  return ;
 }
 while (ctx->a.size > 0)
 {
  pos = find_min_pos(&ctx->a);  // Encontrar el minimo
  rotate_to_top(ctx, pos, 0);   // Llevarlo al tope
  do_pb(ctx);                   // Empujarlo a B
 }
 while (ctx->b.size > 0)
  do_pa(ctx);                   // Devolver todo a A
}
```

> **Verifica:**
>
> ```bash
> ./push_swap --simple 5 3 1 4 2
> ./push_swap --simple 5 3 1 4 2 | wc -l   # Contar operaciones
> ```

---

## Capitulo 12: Algoritmo Medium - Chunk Sort O(n\*sqrt(n))

### El concepto

El selection sort es lento porque busca el minimo **global** cada vez.
El chunk sort divide los numeros en **rangos** (chunks) y los procesa por grupos.

**Fase 1 - Particionar:** Recorrer A y empujar a B todos los numeros del
chunk actual. Si un numero esta por debajo de la mediana del chunk, lo
rotamos en B para mantener los grandes arriba.

**Fase 2 - Reinsertar:** Sacar de B el maximo, rotarlo al tope, empujar a A.
Repetir hasta vaciar B.

```
Ejemplo con n=10 y chunk_size=4:

Numeros normalizados: [7, 2, 9, 0, 5, 3, 8, 1, 6, 4]

Chunk 0: valores 0-3  -> empujar a B
Chunk 1: valores 4-7  -> empujar a B
Chunk 2: valores 8-9  -> empujar a B

Despues de fase 1: A=[]  B=[algo desordenado pero por chunks]
Fase 2: sacar maximos de B -> A=[0,1,2,3,4,5,6,7,8,9]
```

### 12.1 Utilidades: `src/sort_medium_utils.c`

Crea `src/sort_medium_utils.c`:

```c
#include "push_swap.h"

/*
** ft_sqrt_approx: Raiz cuadrada entera aproximada (redondeada arriba).
** Usada para calcular el tamanio del chunk.
*/
int ft_sqrt_approx(int n)
{
 int i;

 i = 1;
 while (i * i < n)
  i++;
 return (i);
}

/*
** get_chunk_size: Calcula el tamanio optimo del chunk.
** Formula empirica: (n * 3) / 100 + 14
** Esta formula fue afinada por prueba y error para minimizar
** operaciones en los rangos que importan (100 y 500 numeros).
**
** Para n=100: chunk_size = 17
** Para n=500: chunk_size = 29
*/
int get_chunk_size(int n)
{
 int size;

 size = (n * 3) / 100 + 14;
 if (size < 5)
  size = 5;
 return (size);
}

/*
** push_back_max: Encuentra el maximo en B, lo rota al tope,
** y lo empuja de vuelta a A.
*/
void push_back_max(t_context *ctx)
{
 int pos;

 pos = find_max_pos(&ctx->b);
 rotate_to_top(ctx, pos, 1);  // 1 = pila B
 do_pa(ctx);
}
```

### 12.2 Algoritmo principal: `src/sort_medium.c`

Crea `src/sort_medium.c`:

```c
#include "push_swap.h"

int  ft_sqrt_approx(int n);
int  get_chunk_size(int n);
void push_back_max(t_context *ctx);

/*
** push_chunk: Empuja a B todos los elementos de A cuyos valores
** estan en el rango [lo, hi].
**
** Optimizacion: si el valor es menor que la mediana del chunk,
** lo rotamos en B (do_rb) para que quede al fondo. Esto mantiene
** los valores grandes mas cerca del tope de B, lo que reduce
** rotaciones en la fase 2.
*/
static void push_chunk(t_context *ctx, int lo, int hi)
{
 int mid;
 int pushed;
 int size;

 mid = (lo + hi) / 2;
 pushed = 0;
 size = ctx->a.size;
 while (pushed < (hi - lo + 1) && size > 0)
 {
  if (ctx->a.data[0] >= lo && ctx->a.data[0] <= hi)
  {
   do_pb(ctx);                        // Empujar a B
   if (ctx->b.data[0] < mid)
    do_rb(ctx);                    // Si es chico, mandarlo al fondo de B
   pushed++;
  }
  else
   do_ra(ctx);                        // Si no es del chunk, rotar A
  size--;
 }
}

/*
** phase_one: Recorre todos los chunks y los empuja a B.
*/
static void phase_one(t_context *ctx, int chunk_sz)
{
 int lo;
 int hi;
 int n;

 n = ctx->a.size;
 lo = 0;
 while (lo < n)
 {
  hi = lo + chunk_sz - 1;
  if (hi >= n)
   hi = n - 1;
  push_chunk(ctx, lo, hi);
  lo = hi + 1;
 }
}

/*
** phase_two: Saca de B el maximo repetidamente hasta vaciar B.
*/
static void phase_two(t_context *ctx)
{
 while (ctx->b.size > 0)
  push_back_max(ctx);
}

/*
** sort_medium: Algoritmo de chunk sort.
*/
void sort_medium(t_context *ctx)
{
 int chunk_sz;

 if (ctx->a.size <= 1 || is_sorted(&ctx->a))
  return ;
 if (ctx->a.size <= 5)
 {
  if (ctx->a.size == 2)
   sort_two(ctx);
  else if (ctx->a.size == 3)
   sort_three(ctx);
  else
   sort_five(ctx);
  return ;
 }
 chunk_sz = get_chunk_size(ctx->a.size);
 phase_one(ctx, chunk_sz);
 phase_two(ctx);
}
```

**Visualizacion del proceso:**

```
Entrada: A=[7,2,9,0,5,3,8,1,6,4]  B=[]

--- Fase 1: Particionar en chunks ---

Chunk [0-3], mid=1:
  7: no esta en [0-3], ra
  2: esta en [0-3], pb -> B=[2]  (2 >= mid=1, no rb)
  9: no, ra
  0: esta, pb -> B=[0,2]  (0 < mid=1, rb -> B=[2,0])
  ...etc

(Despues de procesar todos los chunks)
A=[]  B=[numeros mezclados pero con grandes mas arriba]

--- Fase 2: Reinsertar maximos ---

max en B = 9, rotar al tope, pa -> A=[9]
max en B = 8, rotar al tope, pa -> A=[8,9]
...etc
A=[0,1,2,3,4,5,6,7,8,9]  B=[]
```

> **Verifica:**
>
> ```bash
> ./push_swap --medium $(shuf -i 1-100 -n 100 | tr '\n' ' ') | wc -l
> ```
>
> Deberia dar alrededor de 600-700 operaciones para 100 numeros.

---

## Capitulo 13: Algoritmo Complex - Radix Sort O(n\*log(n))

### El concepto

Radix sort ordena los numeros bit por bit, del bit menos significativo
al mas significativo.

**Prerequisito critico:** Los valores deben estar **normalizados** a `0..n-1`.
Sin normalizacion, los numeros negativos rompen todo.

**Para cada bit (desde bit 0):**

1. Recorrer todos los elementos de A.
2. Si el bit actual del elemento es 1: `ra` (dejarlo en A).
3. Si el bit actual del elemento es 0: `pb` (mandarlo a B).
4. Devolver todo de B a A.

Despues de procesar todos los bits, A esta ordenada.

### Por que funciona?

Piensa en los numeros en binario (despues de normalizar a 0..n-1):

```
Valores normalizados para n=8:
0 = 000
1 = 001
2 = 010
3 = 011
4 = 100
5 = 101
6 = 110
7 = 111

Pasada bit 0: separa pares (bit 0 = 0) de impares (bit 0 = 1)
Pasada bit 1: separa por grupo de 2
Pasada bit 2: separa por grupo de 4
```

Despues de procesar los 3 bits, la estabilidad del radix sort garantiza
que el orden es correcto.

### Crea `src/sort_complex.c`

```c
#include "push_swap.h"

/*
** get_max_bits: Calcula cuantos bits se necesitan para representar n.
** Para n=7 (111 en binario), retorna 3.
** Para n=100, retorna 7 (2^7 = 128 > 100).
*/
static int get_max_bits(int n)
{
 int bits;

 bits = 0;
 while ((n >> bits) != 0)
  bits++;
 return (bits);
}

/*
** radix_pass: Una pasada del radix sort para un bit especifico.
**
** Para cada elemento en A:
**   - Si su bit `bit` es 1: ra (queda en A)
**   - Si su bit `bit` es 0: pb (va a B)
** Luego devuelve todo de B a A.
**
** Ejemplo con bit 0:
**   A=[3,0,2,1] -> en binario: [11, 00, 10, 01]
**   3(11): bit0=1 -> ra   A=[0,2,1,3]
**   0(00): bit0=0 -> pb   A=[2,1,3]  B=[0]
**   2(10): bit0=0 -> pb   A=[1,3]    B=[2,0]
**   1(01): bit0=1 -> ra   A=[3,1]
**   Devolver B: pa,pa -> A=[0,2,3,1]
*/
static void radix_pass(t_context *ctx, int bit)
{
 int size;
 int i;

 size = ctx->a.size;
 i = 0;
 while (i < size)
 {
  if ((ctx->a.data[0] >> bit) & 1)  // El bit es 1?
   do_ra(ctx);                    // Si: queda en A
  else
   do_pb(ctx);                    // No: va a B
  i++;
 }
 while (ctx->b.size > 0)
  do_pa(ctx);                        // Devolver todo a A
}

void sort_complex(t_context *ctx)
{
 int max_bits;
 int bit;

 if (ctx->a.size <= 1 || is_sorted(&ctx->a))
  return ;
 if (ctx->a.size <= 5)
 {
  if (ctx->a.size == 2)
   sort_two(ctx);
  else if (ctx->a.size == 3)
   sort_three(ctx);
  else
   sort_five(ctx);
  return ;
 }
 max_bits = get_max_bits(ctx->a.size - 1);  // n-1 es el valor maximo normalizado
 bit = 0;
 while (bit < max_bits)
 {
  radix_pass(ctx, bit);
  bit++;
 }
}
```

**Complejidad:** Cada pasada recorre n elementos (n operaciones `ra`/`pb` +
hasta n operaciones `pa`). Hay `log2(n)` pasadas. Total: O(n \* log n).

Para n=500: ~9 bits \* ~1000 ops por pasada = ~9000 operaciones.
No es el mas optimo, pero es simple y predecible.

> **Verifica:**
>
> ```bash
> ./push_swap --complex $(shuf -i 1-500 -n 500 | tr '\n' ' ') | wc -l
> ```
>
> Deberia dar alrededor de 5000-7000 operaciones para 500 numeros.

---

## Capitulo 14: Algoritmo Adaptativo

### El concepto

En vez de usar siempre el mismo algoritmo, analizamos **como de desordenado**
esta el input y elegimos la mejor estrategia:

| Condicion       | Estrategia   | Detalle                                 |
| --------------- | ------------ | --------------------------------------- |
| Rotated sorted  | Solo rotar   | O(n) - solo `ra` o `rra`                |
| disorder < 0.2  | Low disorder | O(n) - sacar los pocos desordenados a B |
| disorder < 0.5  | Chunk sort   | O(n\*sqrt(n)) - medium                  |
| disorder >= 0.5 | Radix sort   | O(n\*log(n)) - complex                  |

### Caso especial: Rotated Sorted

Un array "rotated sorted" es uno que estaria ordenado si lo rotaras.
Solo tiene **un punto de quiebre**:

```
[3, 4, 5, 1, 2]  -> rotated sorted (quiebre en 5->1)
[3, 4, 1, 5, 2]  -> NO rotated sorted (quiebres en 4->1 y 5->2)
```

Si detectamos esto, solo necesitamos rotar hasta que el minimo este arriba.

### Caso: Low Disorder (disorder < 0.2)

Si el array esta "casi ordenado" (pocas inversiones):

1. Rotar el minimo al tope de A.
2. Recorrer A: si el elemento esta en orden, `ra`. Si no, `pb`.
3. Reinsertar los elementos de B en sus posiciones correctas.

### Crea `src/sort_adaptive.c`

```c
#include "push_swap.h"

/*
** is_rotated_sorted: Detecta si la pila es una version rotada de
** un array ordenado.
** Cuenta los "quiebres" (donde data[i] > data[i+1]).
** Si hay exactamente 1 quiebre y el ultimo < el primero, es rotated sorted.
** Si hay 0 quiebres, esta directamente ordenado.
*/
static int is_rotated_sorted(t_stack *a)
{
 int breaks;
 int i;

 breaks = 0;
 i = 0;
 while (i < a->size - 1)
 {
  if (a->data[i] > a->data[i + 1])
   breaks++;
  if (breaks > 1)
   return (0);       // Mas de 1 quiebre: no es rotated sorted
  i++;
 }
 if (breaks == 1 && a->data[a->size - 1] > a->data[0])
  return (0);           // El wrap-around no cierra bien
 return (1);
}

/*
** find_insert_pos: Encuentra donde insertar `val` en la pila A
** para mantener el orden. Busca el sucesor mas cercano.
*/
static int find_insert_pos(t_stack *a, int val)
{
 int i;
 int best_pos;
 int best_val;

 best_pos = find_min_pos(a);
 best_val = -1;
 i = 0;
 while (i < a->size)
 {
  if (a->data[i] < val && a->data[i] > best_val)
  {
   best_val = a->data[i];
   best_pos = (i + 1) % a->size;
  }
  i++;
 }
 return (best_pos);
}

/*
** reinsert_sorted: Toma los elementos de B y los inserta en A
** en la posicion correcta. Al final, rota A para que el minimo
** quede arriba.
*/
static void reinsert_sorted(t_context *ctx)
{
 int pos;

 while (ctx->b.size > 0)
 {
  pos = find_max_pos(&ctx->b);
  rotate_to_top(ctx, pos, 1);     // El maximo de B al tope
  pos = find_insert_pos(&ctx->a, ctx->b.data[0]);
  rotate_to_top(ctx, pos, 0);     // Rotar A al punto de insercion
  do_pa(ctx);
 }
 rotate_to_top(ctx, find_min_pos(&ctx->a), 0);  // Minimo al tope
}

/*
** sort_low_disorder: Para arrays casi ordenados.
** 1. Rotar el minimo al tope.
** 2. Recorrer A: si el elemento esta "en su lugar" (>= ultimo visto),
**    ra. Si no, pb (sacarlo a B).
** 3. Reinsertar los de B en orden.
*/
static void sort_low_disorder(t_context *ctx)
{
 int n;
 int last;
 int i;

 rotate_to_top(ctx, find_min_pos(&ctx->a), 0);
 n = ctx->a.size;
 last = -1;
 i = -1;
 while (++i < n)
 {
  if (ctx->a.data[0] >= last)  // Esta en orden?
  {
   last = ctx->a.data[0];
   do_ra(ctx);              // Dejarlo en A
  }
  else
   do_pb(ctx);              // Fuera de orden: a B
 }
 reinsert_sorted(ctx);
}

/*
** sort_adaptive: Punto de entrada del algoritmo adaptativo.
** Analiza el estado de la pila y elige la mejor estrategia.
*/
void sort_adaptive(t_context *ctx)
{
 if (ctx->a.size <= 1 || is_sorted(&ctx->a))
  return ;
 if (ctx->a.size <= 5)
 {
  if (ctx->a.size == 2)
   sort_two(ctx);
  else if (ctx->a.size == 3)
   sort_three(ctx);
  else
   sort_five(ctx);
  return ;
 }
 if (is_rotated_sorted(&ctx->a))
 {
  rotate_to_top(ctx, find_min_pos(&ctx->a), 0);
  return ;
 }
 if (ctx->disorder < 0.2)
  sort_low_disorder(ctx);
 else if (ctx->disorder < 0.5)
  sort_medium(ctx);
 else
  sort_complex(ctx);
}
```

**Tabla de decision completa:**

```
  input
    |
  size <= 5? --si--> sort_two/three/five
    |
  ya ordenado? --si--> return
    |
  rotated sorted? --si--> rotar al minimo (O(n))
    |
  disorder < 0.2? --si--> sort_low_disorder (O(n) aprox)
    |
  disorder < 0.5? --si--> sort_medium / chunk sort (O(n*sqrt(n)))
    |
  sort_complex / radix sort (O(n*log(n)))
```

> **Verifica:**
>
> ```bash
> # Array casi ordenado (low disorder):
> ./push_swap 1 2 3 5 4 6 7 8 9 10 | wc -l
>
> # Array rotado:
> ./push_swap 5 6 7 8 9 10 1 2 3 4 | wc -l
>
> # Array completamente aleatorio:
> ./push_swap $(shuf -i 1-100 -n 100 | tr '\n' ' ') | wc -l
> ```

---

## Capitulo 15: Benchmark

El flag `--bench` muestra estadisticas utiles por stderr (para no contaminar
la salida de operaciones en stdout).

### Crea `src/bench.c`

```c
#include "push_swap.h"

/*
** print_disorder: Imprime el porcentaje de desorden.
** Como no tenemos printf con %f en 42, lo hacemos manualmente:
** disorder=0.4523 -> "45.23%"
*/
static void print_disorder(t_context *ctx)
{
 int pct_int;
 int pct_dec;

 pct_int = (int)(ctx->disorder * 100);          // Parte entera: 45
 pct_dec = (int)(ctx->disorder * 10000) % 100;  // Parte decimal: 23
 ft_putstr_fd("[bench] disorder: ", 2);
 ft_putnbr_fd(pct_int, 2);
 ft_putstr_fd(".", 2);
 if (pct_dec < 10)
  ft_putstr_fd("0", 2);    // Pad con 0: "5" -> "05"
 ft_putnbr_fd(pct_dec, 2);
 ft_putstr_fd("%\n", 2);
}

/*
** strat_name: Retorna el nombre de la estrategia usada.
*/
static const char *strat_name(int strat, double disorder)
{
 if (strat == STRAT_SIMPLE)
  return ("Simple / O(n^2)");
 if (strat == STRAT_MEDIUM)
  return ("Medium / O(n*sqrt(n))");
 if (strat == STRAT_COMPLEX)
  return ("Complex / O(n*log(n))");
 if (disorder < 0.2)
  return ("Adaptive / O(n)");
 if (disorder < 0.5)
  return ("Adaptive / O(n*sqrt(n))");
 return ("Adaptive / O(n*log(n))");
}

static void print_strategy(t_context *ctx)
{
 ft_putstr_fd("[bench] strategy: ", 2);
 ft_putstr_fd((char *)strat_name(ctx->strategy, ctx->disorder), 2);
 ft_putstr_fd("\n", 2);
 ft_putstr_fd("[bench] total_ops: ", 2);
 ft_putnbr_fd(ctx->total_ops, 2);
 ft_putstr_fd("\n", 2);
}

/*
** print_ops_line: Imprime un rango de contadores de operaciones.
*/
static void print_ops_line(t_context *ctx, int start, int end)
{
 static const char *names[] = {"sa", "sb", "ss", "pa", "pb",
  "ra", "rb", "rr", "rra", "rrb", "rrr"};
 int     i;

 ft_putstr_fd("[bench] ", 2);
 i = start;
 while (i <= end)
 {
  ft_putstr_fd((char *)names[i], 2);
  ft_putstr_fd(": ", 2);
  ft_putnbr_fd(ctx->ops[i], 2);
  if (i < end)
   ft_putstr_fd("  ", 2);
  i++;
 }
 ft_putstr_fd("\n", 2);
}

void print_bench(t_context *ctx)
{
 print_disorder(ctx);
 print_strategy(ctx);
 print_ops_line(ctx, 0, 4);
 print_ops_line(ctx, 5, 10);
}
```

> **Verifica:**
>
> ```bash
> ./push_swap --bench $(shuf -i 1-100 -n 100 | tr '\n' ' ') > /dev/null
> ```
>
> Deberia mostrar algo como:
>
> ```
> [bench] disorder: 48.72%
> [bench] strategy: Adaptive / O(n*sqrt(n))
> [bench] total_ops: 653
> [bench] sa: 0  sb: 0  ss: 0  pa: 100  pb: 100
> [bench] ra: 180  rb: 42  rr: 0  rra: 120  rrb: 111  rrr: 0
> ```

---

## Capitulo 16 (Bonus): El Checker

El checker es un programa separado que:

1. Recibe los mismos numeros que push_swap.
2. Lee operaciones de stdin (una por linea).
3. Las ejecuta sobre las pilas.
4. Verifica si al final A esta ordenada y B vacia.
5. Imprime `OK` o `KO`.

### Uso tipico

```bash
# Verificar que push_swap produce una solucion correcta:
./push_swap 3 2 1 | ./checker 3 2 1
# Debe imprimir: OK

# Verificar manualmente (escribir operaciones a mano):
./checker 3 2 1
sa
rra
(presionar Ctrl+D)
# Debe imprimir: OK
```

### 16.1 Ejecucion de operaciones: `bonus/checker_exec_bonus.c`

Crea `bonus/checker_exec_bonus.c`:

```c
#include "push_swap.h"
#include "../libft/get_next_line.h"

/*
** exec_swap_push: Intenta ejecutar una operacion de swap o push.
** Compara la linea leida (incluye '\n') contra los nombres conocidos.
** Retorna 1 si reconocio la operacion, 0 si no.
**
** Nota: Usamos las funciones op_* directamente (sin do_*) porque
** el checker NO debe imprimir las operaciones.
*/
static int exec_swap_push(t_context *ctx, char *line)
{
 if (ft_strncmp(line, "sa\n", 3) == 0)
  op_sa(&ctx->a);
 else if (ft_strncmp(line, "sb\n", 3) == 0)
  op_sb(&ctx->b);
 else if (ft_strncmp(line, "ss\n", 3) == 0)
  op_ss(&ctx->a, &ctx->b);
 else if (ft_strncmp(line, "pa\n", 3) == 0)
  op_pa(&ctx->a, &ctx->b);
 else if (ft_strncmp(line, "pb\n", 3) == 0)
  op_pb(&ctx->a, &ctx->b);
 else
  return (0);
 return (1);
}

/*
** exec_rotate: Igual pero para operaciones de rotacion.
** Nota: comparamos rra/rrb/rrr ANTES que ra/rb/rr para evitar
** que "rra\n" matchee con "rr" (porque ft_strncmp con n=3
** matchearia "rra" con "rr\n" incorrectamente... no en este caso
** pero el orden es mas seguro).
*/
static int exec_rotate(t_context *ctx, char *line)
{
 if (ft_strncmp(line, "rra\n", 4) == 0)
  op_rra(&ctx->a);
 else if (ft_strncmp(line, "rrb\n", 4) == 0)
  op_rrb(&ctx->b);
 else if (ft_strncmp(line, "rrr\n", 4) == 0)
  op_rrr(&ctx->a, &ctx->b);
 else if (ft_strncmp(line, "ra\n", 3) == 0)
  op_ra(&ctx->a);
 else if (ft_strncmp(line, "rb\n", 3) == 0)
  op_rb(&ctx->b);
 else if (ft_strncmp(line, "rr\n", 3) == 0)
  op_rr(&ctx->a, &ctx->b);
 else
  return (0);
 return (1);
}

/*
** exec_operation: Intenta ejecutar una operacion leida.
*/
int exec_operation(t_context *ctx, char *line)
{
 if (exec_swap_push(ctx, line))
  return (1);
 if (exec_rotate(ctx, line))
  return (1);
 return (0);     // Operacion no reconocida -> error
}

/*
** read_and_exec: Lee operaciones de stdin hasta EOF.
** Para cada linea, intenta ejecutarla. Si falla, retorna 0.
*/
int read_and_exec(t_context *ctx)
{
 char *line;

 line = get_next_line(0);
 while (line)
 {
  if (!exec_operation(ctx, line))
  {
   free(line);
   return (0);    // Operacion invalida
  }
  free(line);
  line = get_next_line(0);
 }
 return (1);
}
```

### 16.2 Main del checker: `bonus/checker_main_bonus.c`

Crea `bonus/checker_main_bonus.c`:

```c
#include "push_swap.h"
#include "../libft/get_next_line.h"

int  exec_operation(t_context *ctx, char *line);
int  read_and_exec(t_context *ctx);

static void checker_error_exit(t_context *ctx)
{
 free_context(ctx);
 write(2, "Error\n", 6);
 exit(1);
}

static void init_checker_ctx(t_context *ctx)
{
 int i;

 ctx->total_ops = 0;
 ctx->strategy = 0;
 ctx->bench = 0;
 ctx->disorder = 0.0;
 i = 0;
 while (i < 11)
  ctx->ops[i++] = 0;
}

/*
** check_result: Si A esta ordenada y B esta vacia -> OK. Si no -> KO.
*/
static void check_result(t_context *ctx)
{
 if (is_sorted(&ctx->a) && ctx->b.size == 0)
  write(1, "OK\n", 3);
 else
  write(1, "KO\n", 3);
}

int main(int argc, char **argv)
{
 t_context ctx;

 if (argc < 2)
  return (0);
 init_checker_ctx(&ctx);
 if (!stack_create(&ctx.a, 10000))
  return (1);
 if (!stack_create(&ctx.b, 10000))
 {
  stack_destroy(&ctx.a);
  return (1);
 }
 if (!parse_args(argc, argv, &ctx))
  checker_error_exit(&ctx);
 if (ctx.a.size == 0)
 {
  free_context(&ctx);
  return (0);
 }
 if (!read_and_exec(&ctx))
  checker_error_exit(&ctx);
 check_result(&ctx);
 free_context(&ctx);
 return (0);
}
```

**Diferencias con el main de push_swap:**

- No normaliza (no lo necesita).
- No calcula desorden.
- No ejecuta ningun algoritmo de sort.
- En vez de eso, lee operaciones de stdin y las ejecuta.
- Al final, verifica el resultado.

> **Verifica:**
>
> ```bash
> make bonus
>
> # Test basico:
> ./push_swap 3 2 1 | ./checker 3 2 1
> # Debe imprimir: OK
>
> # Test con mas numeros:
> ARG=$(shuf -i 1-100 -n 100 | tr '\n' ' ')
> ./push_swap $ARG | ./checker $ARG
> # Debe imprimir: OK
>
> # Test manual incorrecto:
> echo "sa" | ./checker 3 2 1
> # Debe imprimir: KO (sa solo no ordena 3 2 1)
> ```

---

## Capitulo 17: Pruebas y Optimizacion

### Limites del proyecto 42

| n   | Maximo operaciones | Puntuacion  |
| --- | ------------------ | ----------- |
| 3   | <= 3               | Obligatorio |
| 5   | <= 12              | Obligatorio |
| 100 | <= 700             | 5 puntos    |
| 500 | <= 5500            | 5 puntos    |

### Script de pruebas automaticas

Crea un script `test.sh` (no es entregable, solo para ti):

```bash
#!/bin/bash

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Test con n numeros
test_n() {
    local n=$1
    local max_ops=$2
    local iterations=$3
    local total=0
    local max=0
    local fails=0

    echo "=== Testing n=$n (max allowed: $max_ops) ==="

    for i in $(seq 1 $iterations); do
        ARG=$(shuf -i 1-$((n * 10)) -n $n | tr '\n' ' ')
        OPS=$(./push_swap $ARG | wc -l)
        RESULT=$(./push_swap $ARG | ./checker $ARG)

        total=$((total + OPS))
        if [ $OPS -gt $max ]; then
            max=$OPS
        fi

        if [ "$RESULT" != "OK" ]; then
            fails=$((fails + 1))
            echo -e "${RED}FAIL${NC}: $ARG ($OPS ops, result: $RESULT)"
        fi

        if [ $OPS -gt $max_ops ]; then
            echo -e "${RED}OVER LIMIT${NC}: $OPS ops (max: $max_ops)"
        fi
    done

    local avg=$((total / iterations))
    echo "  Average: $avg ops"
    echo "  Max:     $max ops"
    echo "  Fails:   $fails / $iterations"
    if [ $max -le $max_ops ] && [ $fails -eq 0 ]; then
        echo -e "  ${GREEN}PASS${NC}"
    else
        echo -e "  ${RED}NEEDS WORK${NC}"
    fi
    echo ""
}

# Compilar
make re

# Tests
test_n 3 3 100
test_n 5 12 100
test_n 100 700 50
test_n 500 5500 10
```

```bash
chmod +x test.sh
./test.sh
```

### Casos edge que debes probar

```bash
# Sin argumentos (no imprime nada, exit 0)
./push_swap

# Un solo numero (no imprime nada)
./push_swap 42

# Ya ordenado (no imprime nada)
./push_swap 1 2 3 4 5

# Numeros negativos
./push_swap -5 -3 -1 0 2 4

# Limites de int
./push_swap 2147483647 -2147483648 0

# Errores (deben imprimir "Error\n" a stderr)
./push_swap 2147483648        # Overflow
./push_swap abc               # No es numero
./push_swap 1 2 3 1           # Duplicado
./push_swap ""                # String vacio
./push_swap "1 2 3 a"         # Mixto invalido
```

### Consejos de optimizacion

1. **El chunk_size importa mucho.** La formula `(n * 3) / 100 + 14` fue
   afinada empiricamente. Puedes experimentar con otros valores.

2. **rotate_to_top siempre elige la direccion mas corta.** Esto ya esta
   optimizado en nuestro codigo.

3. **La optimizacion del chunk sort** con `do_rb` cuando el valor es menor
   que la mediana del chunk reduce significativamente las operaciones en la
   fase 2.

4. **El algoritmo adaptativo** es tu mejor apuesta para la evaluacion de 42
   porque elige la estrategia optima segun el input.

5. **Para conseguir menos de 700 ops en n=100:** El chunk sort suele estar
   en el rango 550-700. Si necesitas bajar mas, investiga el algoritmo
   "Turkish sort" (aka "cost-based insertion sort").

6. **Para n=500 en menos de 5500:** El chunk sort con buenos parametros o
   el radix sort deberian cumplir.

### Flujo de compilacion final

```bash
# Compilar todo
make re

# Compilar bonus
make bonus

# Test rapido
./push_swap 3 2 1
# Esperado: sa  rra

# Test con checker
ARG="5 3 1 4 2"
./push_swap $ARG | ./checker $ARG
# Esperado: OK

# Benchmark
./push_swap --bench $(shuf -i 1-500 -n 500 | tr '\n' ' ') > /dev/null
```

---

## Resumen: Orden de construccion

Si seguiste el tutorial paso a paso, creaste los archivos en este orden:

1. `includes/push_swap.h` - El mapa completo
2. `Makefile` - El sistema de build
3. `src/stack_init.c` - Crear y destruir pilas
4. `src/stack_ops_swap.c` - sa, sb, ss
5. `src/stack_ops_push.c` - pa, pb
6. `src/stack_ops_rotate.c` - ra, rb, rr
7. `src/stack_ops_revrot.c` - rra, rrb, rrr
8. `src/ops_out_swap.c` - Wrappers con salida (swap/push)
9. `src/ops_out_rot.c` - Wrappers con salida (rotate)
10. `src/ops_out_revrot.c` - Wrappers con salida (reverse rotate)
11. `src/parse_utils.c` - Validacion de enteros
12. `src/parse_args.c` - Parser de argumentos
13. `src/main.c` - Punto de entrada
14. `src/utils.c` - is_sorted, find_min/max, rotate_to_top
15. `src/normalize.c` - Normalizacion a 0..n-1
16. `src/disorder.c` - Metrica de inversiones
17. `src/sort_tiny.c` - Sort para 2, 3 y 5 elementos
18. `src/sort_simple.c` - Selection sort O(n^2)
19. `src/sort_medium_utils.c` - Helpers del chunk sort
20. `src/sort_medium.c` - Chunk sort O(n\*sqrt(n))
21. `src/sort_complex.c` - Radix sort O(n\*log(n))
22. `src/sort_adaptive.c` - Selector adaptativo
23. `src/bench.c` - Benchmark
24. `bonus/checker_exec_bonus.c` - Ejecutor de operaciones
25. `bonus/checker_main_bonus.c` - Main del checker

**Hitos de compilacion:**

- Despues del archivo 13 (`main.c`): primera compilacion funcional.
- Despues del archivo 17 (`sort_tiny.c`): ordena 2, 3 y 5 elementos.
- Despues del archivo 22 (`sort_adaptive.c`): ordena cualquier cantidad.
- Despues del archivo 25: checker funcional.

---

_Tutorial creado para el proyecto push_swap de 42 Madrid._
