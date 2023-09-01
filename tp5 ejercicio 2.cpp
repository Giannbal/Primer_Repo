#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct medicion {
	char fecha[20];
	char sensor[20];
	float valor;
	struct medicion *prev;
	struct medicion *next;
};

bool esFechaValida(int dia, int mes, int anio);
void agregar(struct medicion **head, struct medicion **tail);
void eliminar(struct medicion **head, struct medicion **tail);
void mostrar(struct medicion *head);
void guardarEnArchivo(struct medicion *head);
void cargarDesdeArchivo(struct medicion **head, struct medicion **tail);
void buscarPorSensor(struct medicion *head);
void buscarPorValor(struct medicion *head);

int main() {
	struct medicion *head = NULL;
	struct medicion *tail = NULL;
	
	cargarDesdeArchivo(&head, &tail);
	
	int opcion;
	do {
		printf("\n¿Qué desea hacer?\n");
		printf("1. Agregar una nueva medición\n");
		printf("2. Eliminar mediciones existentes\n");
		printf("3. Mostrar mediciones\n");
		printf("4. Buscar mediciones por sensor\n");
		printf("5. Buscar mediciones por valor\n");
		printf("6. Guardar mediciones en archivo\n");
		printf("7. Salir\n");
		printf("Ingrese su opción: ");
		scanf("%d", &opcion);
		getchar();
		
		switch (opcion) {
		case 1:
			agregar(&head, &tail);
			break;
		case 2:
			eliminar(&head, &tail);
			break;
		case 3:
			mostrar(head);
			break;
		case 4:
			buscarPorSensor(head);
			break;
		case 5:
			buscarPorValor(head);
			break;
		case 6:
			guardarEnArchivo(head);
			break;
		case 7:
			// Clean up memory before exiting
			while (head != NULL) {
				struct medicion *temp = head;
				head = head->next;
				free(temp);
			}
			break;
		default:
			printf("Opción inválida. Intente nuevamente.\n");
		}
	} while (opcion != 7);
	
	return 0;
}

bool esFechaValida(int dia, int mes, int anio) {
	// Implementation of date validation
	// For the sake of example, consider all dates valid
	return true;
}

void agregar(struct medicion **head, struct medicion **tail) {
	struct medicion *nuevo = (struct medicion *)malloc(sizeof(struct medicion));
	printf("Ingrese una fecha en formato dd/mm/aaaa: ");
	scanf("%s", nuevo->fecha);
	printf("Ingrese el nombre del sensor: ");
	scanf("%s", nuevo->sensor);
	printf("Ingrese el valor medido: ");
	scanf("%f", &nuevo->valor);
	
	nuevo->prev = *tail;
	nuevo->next = NULL;
	
	if (*tail != NULL) {
		(*tail)->next = nuevo;
	} else {
		*head = nuevo;
	}
	
	*tail = nuevo;
}

void eliminar(struct medicion **head, struct medicion **tail) {
	int posicion;
	printf("Ingrese la posición de la medición a eliminar: ");
	scanf("%d", &posicion);
	
	struct medicion *actual = *head;
	int i = 1;
	while (actual != NULL && i < posicion) {
		actual = actual->next;
		i++;
	}
	
	if (actual == NULL) {
		printf("Posición inválida.\n");
		return;
	}
	
	if (actual->prev != NULL) {
		actual->prev->next = actual->next;
	} else {
		*head = actual->next;
	}
	
	if (actual->next != NULL) {
		actual->next->prev = actual->prev;
	} else {
		*tail = actual->prev;
	}
	
	free(actual);
	printf("Medición eliminada.\n");
}

void mostrar(struct medicion *head) {
	printf("\nListado de mediciones:\n");
	struct medicion *actual = head;
	while (actual != NULL) {
		printf("Fecha: %s | Sensor: %s | Valor: %.2f\n", actual->fecha, actual->sensor, actual->valor);
		actual = actual->next;
	}
}

void guardarEnArchivo(struct medicion *head) {
	FILE *archivo = fopen("mediciones.bin", "wb");
	if (archivo == NULL) {
		printf("No se pudo abrir el archivo.\n");
		return;
	}
	
	struct medicion *actual = head;
	while (actual != NULL) {
		fwrite(actual, sizeof(struct medicion), 1, archivo);
		actual = actual->next;
	}
	
	fclose(archivo);
	printf("Mediciones guardadas en el archivo.\n");
}

void cargarDesdeArchivo(struct medicion **head, struct medicion **tail) {
	FILE *archivo = fopen("mediciones.bin", "rb");
	if (archivo == NULL) {
		printf("No se pudo abrir el archivo de mediciones.\n");
		return;
	}
	
	while (1) {
		struct medicion *nuevo = (struct medicion *)malloc(sizeof(struct medicion));
		if (fread(nuevo, sizeof(struct medicion), 1, archivo) != 1) {
			free(nuevo);
			break;
		}
		
		nuevo->prev = *tail;
		nuevo->next = NULL;
		
		if (*tail != NULL) {
			(*tail)->next = nuevo;
		} else {
			*head = nuevo;
		}
		
		*tail = nuevo;
	}
	
	fclose(archivo);
}

void buscarPorSensor(struct medicion *head) {
	char sensorBuscado[20];
	printf("Ingrese el nombre del sensor a buscar: ");
	scanf("%s", sensorBuscado);
	
	printf("\nResultados de la búsqueda por sensor:\n");
	struct medicion *actual = head;
	while (actual != NULL) {
		if (strcmp(actual->sensor, sensorBuscado) == 0) {
			printf("Fecha: %s | Sensor: %s | Valor: %.2f\n", actual->fecha, actual->sensor, actual->valor);
		}
		actual = actual->next;
	}
}

void buscarPorValor(struct medicion *head) {
	float valorUmbral;
	int opcion;
	printf("Ingrese el valor umbral a buscar: ");
	scanf("%f", &valorUmbral);
	
	printf("¿Desea buscar mediciones que superen (1) o igualen o superen (2) el umbral? ");
	scanf("%d", &opcion);
	
	printf("\nResultados de la búsqueda por valor:\n");
	struct medicion *actual = head;
	while (actual != NULL) {
		if (opcion == 1 && actual->valor > valorUmbral) {
			printf("Fecha: %s | Sensor: %s | Valor: %.2f\n", actual->fecha, actual->sensor, actual->valor);
		} else if (opcion == 2 && actual->valor >= valorUmbral) {
			printf("Fecha: %s | Sensor: %s | Valor: %.2f\n", actual->fecha, actual->sensor, actual->valor);
		}
		actual = actual->next;
	}
}
