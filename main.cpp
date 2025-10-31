/**
 * @file main.cpp
 * @brief Sistema de Gestión Polimórfica de Sensores
 * @author Jesus Alberto Tovar Ramirez
 * @date 29/10/2025
 */

#include <iostream>
#include <cstring>

// ============================================================================
// LISTA ENLAZADA GENERICA
// ============================================================================

/**
 * @brief Nodo generico para la lista enlazada
 * @tparam T es Tipo de dato que almacena el nodo
 */
template <typename T>
struct Nodo {
    T dato;
    Nodo<T>* siguiente;
    
    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

/**
 * @brief Lista enlazada simple generica
 * @tparam T Tipo de dato que almacenara la lista
 */
template <typename T>
class ListaSensor {
private:
    Nodo<T>* cabeza;
    int tamanio;

public:
    ListaSensor() : cabeza(nullptr), tamanio(0) {}
    
    ~ListaSensor() {
        std::cout << "  [Destructor ListaSensor] Liberando lista interna..." << std::endl;
        while (cabeza != nullptr) {
            Nodo<T>* temp = cabeza;
            cabeza = cabeza->siguiente;
            std::cout << "    [Log] Nodo<T> " << temp->dato << " liberado." << std::endl;
            delete temp;
        }
    }
    
    ListaSensor(const ListaSensor& otra) : cabeza(nullptr), tamanio(0) {
        Nodo<T>* actual = otra.cabeza;
        while (actual != nullptr) {
            insertar(actual->dato);
            actual = actual->siguiente;
        }
    }
    
    ListaSensor& operator=(const ListaSensor& otra) {
        if (this != &otra) {
            while (cabeza != nullptr) {
                Nodo<T>* temp = cabeza;
                cabeza = cabeza->siguiente;
                delete temp;
            }
            
            tamanio = 0;
            Nodo<T>* actual = otra.cabeza;
            while (actual != nullptr) {
                insertar(actual->dato);
                actual = actual->siguiente;
            }
        }
        return *this;
    }
    
    void insertar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        std::cout << "[Log] Insertando Nodo<T> con valor: " << valor << std::endl;
        
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            Nodo<T>* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        tamanio++;
    }
    
    int obtenerTamanio() const {
        return tamanio;
    }
    
    T calcularPromedio() const {
        if (tamanio == 0) return T(0);
        
        T suma = T(0);
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            suma = suma + actual->dato;
            actual = actual->siguiente;
        }
        return suma / tamanio;
    }
    
    T eliminarMinimo() {
        if (cabeza == nullptr) return T(0);
        
        Nodo<T>* actual = cabeza;
        Nodo<T>* nodoMin = cabeza;
        Nodo<T>* prevMin = nullptr;
        Nodo<T>* prevActual = nullptr;
        
        while (actual != nullptr) {
            if (actual->dato < nodoMin->dato) {
                nodoMin = actual;
                prevMin = prevActual;
            }
            prevActual = actual;
            actual = actual->siguiente;
        }
        
        T valorMin = nodoMin->dato;
        
        if (prevMin == nullptr) {
            cabeza = nodoMin->siguiente;
        } else {
            prevMin->siguiente = nodoMin->siguiente;
        }
        
        delete nodoMin;
        tamanio--;
        
        return valorMin;
    }
};

// ============================================================================
// CLASE BASE ABSTRACTA
// ============================================================================

/**
 * @brief Clase abstracta base para sensores
 */
class SensorBase {
protected:
    char nombre[50];
    
public:
    SensorBase(const char* nombreS) {
        std::strncpy(nombre, nombreS, 49);
        nombre[49] = '\0';
    }
    
    virtual ~SensorBase() {
        std::cout << "[Destructor SensorBase] Sensor " << nombre << " destruido." << std::endl;
    }
    
    virtual void procesarLectura() = 0;
    virtual void imprimirInfo() const = 0;
    
    const char* obtenerNombre() const {
        return nombre;
    }
};

// ============================================================================
// SENSOR DE TEMPERATURA
// ============================================================================

/**
 * @brief Sensor que maneja lecturas de temperatura pos el float
 */
class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float> historial;
    
public:
    SensorTemperatura(const char* nombreS) : SensorBase(nombreS) {}
    
    ~SensorTemperatura() {
        std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna..." << std::endl;
    }
    
    void registrarLectura(float valor) {
        std::cout << "[Log] Registrando lectura en " << nombre << ": " << valor << std::endl;
        historial.insertar(valor);
    }
    
    void procesarLectura() override {
        std::cout << "-> Procesando Sensor " << nombre << "..." << std::endl;
        
        if (historial.obtenerTamanio() == 0) {
            std::cout << "[Sensor Temp] No hay lecturas para procesar." << std::endl;
            return;
        }
        
        float minimo = historial.eliminarMinimo();
        std::cout << "[Sensor Temp] Lectura más baja (" << minimo << ") eliminada.";
        
        if (historial.obtenerTamanio() > 0) {
            float promedio = historial.calcularPromedio();
            std::cout << " Promedio restante: " << promedio << std::endl;
        } else {
            std::cout << " No quedan lecturas." << std::endl;
        }
    }
    
    void imprimirInfo() const override {
        std::cout << "[Sensor Temperatura] ID: " << nombre 
                  << ", Lecturas: " << historial.obtenerTamanio() << std::endl;
    }
};

// ============================================================================
// SENSOR DE PRESI=N
// ============================================================================

/**
 * @brief Sensor que maneja lecturas de presion el int
 */
class SensorPresion : public SensorBase {
private:
    ListaSensor<int> historial;
    
public:
    SensorPresion(const char* nombreS) : SensorBase(nombreS) {}
    
    ~SensorPresion() {
        std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna..." << std::endl;
    }
    
    void registrarLectura(int valor) {
        std::cout << "[Log] Registrando lectura en " << nombre << ": " << valor << std::endl;
        historial.insertar(valor);
    }
    
    void procesarLectura() override {
        std::cout << "-> Procesando Sensor " << nombre << "..." << std::endl;
        
        if (historial.obtenerTamanio() == 0) {
            std::cout << "[Sensor Presion] No hay lecturas para procesar." << std::endl;
            return;
        }
        
        int promedio = historial.calcularPromedio();
        std::cout << "[Sensor Presion] Promedio de lecturas: " << promedio << std::endl;
    }
    
    void imprimirInfo() const override {
        std::cout << "[Sensor Presion] ID: " << nombre 
                  << ", Lecturas: " << historial.obtenerTamanio() << std::endl;
    }
};

// ============================================================================
// LISTA GENERAL (NO GENERICA) PARA POLIMORFISMO
// ============================================================================

/**
 * @brief Nodo para la lista de gestion polimorfica
 */
struct NodoGeneral {
    SensorBase* sensor;
    NodoGeneral* siguiente;
    
    NodoGeneral(SensorBase* s) : sensor(s), siguiente(nullptr) {}
};

/**
 * @brief Lista de gestion que almacena punteros a sensorbase
 */
class ListaGeneral {
private:
    NodoGeneral* cabeza;
    
public:
    ListaGeneral() : cabeza(nullptr) {}
    
    ~ListaGeneral() {
        std::cout << "\n--- Liberación de Memoria en Cascada ---" << std::endl;
        while (cabeza != nullptr) {
            NodoGeneral* temp = cabeza;
            std::cout << "[Destructor General] Liberando Nodo: " 
                      << temp->sensor->obtenerNombre() << std::endl;
            delete temp->sensor;
            cabeza = cabeza->siguiente;
            delete temp;
        }
    }
    
    void insertar(SensorBase* sensor) {
        NodoGeneral* nuevo = new NodoGeneral(sensor);
        
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            NodoGeneral* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        
        std::cout << "Sensor '" << sensor->obtenerNombre() 
                  << "' insertado en la lista de gestión." << std::endl;
    }
    
    SensorBase* buscar(const char* nombreBuscar) {
        NodoGeneral* actual = cabeza;
        while (actual != nullptr) {
            if (std::strcmp(actual->sensor->obtenerNombre(), nombreBuscar) == 0) {
                return actual->sensor;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    void procesarTodos() {
        std::cout << "\n--- Ejecutando Polimorfismo ---" << std::endl;
        NodoGeneral* actual = cabeza;
        while (actual != nullptr) {
            actual->sensor->procesarLectura();
            actual = actual->siguiente;
        }
    }
    
    void imprimirTodos() const {
        std::cout << "\n--- Lista de Sensores Registrados ---" << std::endl;
        NodoGeneral* actual = cabeza;
        while (actual != nullptr) {
            actual->sensor->imprimirInfo();
            actual = actual->siguiente;
        }
    }
};

// ============================================================================
// FUNCION PRINCIPAL
// ============================================================================

int main() {
    ListaGeneral sistema;
    int opcion;
    
    std::cout << "=== Sistema IoT de Monitoreo Polimórfico ===" << std::endl;
    
    do {
        std::cout << "\n--- MENÚ ---" << std::endl;
        std::cout << "1. Crear Sensor de Temperatura" << std::endl;
        std::cout << "2. Crear Sensor de Presión" << std::endl;
        std::cout << "3. Registrar Lectura" << std::endl;
        std::cout << "4. Ejecutar Procesamiento Polimórfico" << std::endl;
        std::cout << "5. Mostrar Sensores" << std::endl;
        std::cout << "0. Salir" << std::endl;
        std::cout << "Opción: ";
        std::cin >> opcion;
        
        switch(opcion) {
            case 1: {
                char id[50];
                std::cout << "ID del sensor: ";
                std::cin >> id;
                SensorTemperatura* temp = new SensorTemperatura(id);
                sistema.insertar(temp);
                break;
            }
            case 2: {
                char id[50];
                std::cout << "ID del sensor: ";
                std::cin >> id;
                SensorPresion* pres = new SensorPresion(id);
                sistema.insertar(pres);
                break;
            }
            case 3: {
                char id[50];
                std::cout << "ID del sensor: ";
                std::cin >> id;
                
                SensorBase* sensor = sistema.buscar(id);
                if (sensor == nullptr) {
                    std::cout << "Sensor no encontrado." << std::endl;
                    break;
                }
                
                SensorTemperatura* temp = dynamic_cast<SensorTemperatura*>(sensor);
                if (temp != nullptr) {
                    float valor;
                    std::cout << "Valor (float): ";
                    std::cin >> valor;
                    temp->registrarLectura(valor);
                } else {
                    SensorPresion* pres = dynamic_cast<SensorPresion*>(sensor);
                    if (pres != nullptr) {
                        int valor;
                        std::cout << "Valor (int): ";
                        std::cin >> valor;
                        pres->registrarLectura(valor);
                    }
                }
                break;
            }
            case 4:
                sistema.procesarTodos();
                break;
            case 5:
                sistema.imprimirTodos();
                break;
            case 0:
                std::cout << "\nCerrando sistema..." << std::endl;
                break;
            default:
                std::cout << "Opción inválida." << std::endl;
        }
    } while(opcion != 0);
    
    return 0;
}