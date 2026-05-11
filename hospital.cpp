/*
 * ============================================================
 * SISTEMA DE GESTION HOSPITALARIA
 * Materia: Programacion Orientada a Objetos
 * Autor: Humberto Hernandez Nonigo
 * Descripcion: Administracion de pacientes, medicos y citas
 * ============================================================
 */

#include <iostream>   // Entrada/salida estandar
#include <vector>     // Contenedor dinamico para listas
#include <string>     // Manejo de cadenas
#include <algorithm>  // Para ordenar (sort)
#include <fstream>    // Para escritura/lectura de archivos
#include <sstream>    // Para construir cadenas con formato
#include <limits>     // Para limites de tipos de datos (limpiar buffer)

using namespace std;

// ============================================================
// PLANTILLA (TEMPLATE): Repositorio que permite almacenar cualquier tipo T en un vector dinamico
// Genera clases concretas al especificar el tipo T.
// ============================================================
template <typename T>
class Repositorio {
private:
    // [Librería <vector>] Uso de la plantilla vector como contenedor dinámico para almacenar punteros
    vector<T*> elementos; // Vector de punteros al tipo generico T

public:
    // Constructor: inicializa el repositorio vacio
    Repositorio() {
        cout << "\n Construccion de la Plantilla" << endl;
    }

    // Destructor: libera la memoria de todos los elementos almacenados
    ~Repositorio() {
        for (T* e : elementos)  // Itera sobre cada puntero
            delete e;           // Libera la memoria apuntada

        // [Librería <vector>] Uso de clear() para vaciar todos los elementos del arreglo dinámico
        elementos.clear();      // Vacia el vector
        cout <<"Destruccion de la Plantilla" << endl;
    }

    // Agrega un nuevo elemento al repositorio
    void agregar(T* elem) {
        // [Librería <vector>] Uso de push_back() para insertar un elemento al final del vector
        elementos.push_back(elem); // Inserta al final del vector
    }

    // Devuelve el vector completo de elementos (solo lectura)
    const vector<T*>& obtenerTodos() const {
        return elementos;
    }

    // Devuelve el numero de elementos almacenados
    int cantidad() const {
        // [Librería <vector>] Uso de size() para obtener el tamaño actual del contenedor
        return (int)elementos.size();
    }
};

//HERENCIA JERARQUICA
// CLASE BASE ABSTRACTA: Persona
// Contiene los atributos comunes de medicos y pacientes

class Persona {
protected:
    // [Librería <string>] Uso de string para manejar cadenas de texto dinámicas en lugar de arreglos de char
    string nombre;    // Nombre completo de la persona
    int    edad;      // Edad en anios
    string domicilio; // Direccion de residencia
    string telefono;  // Numero de telefono de contacto

public:
    // Constructor con parametros: inicializa todos los atributos
    Persona(const string& nombre, int edad,
            const string& domicilio, const string& telefono)
        : nombre(nombre), edad(edad),
          domicilio(domicilio), telefono(telefono) {
        cout << "\n Construccion de la Clase Base" << endl;
    }

    // Destructor virtual: necesario para liberar correctamente
    // cuando se usa polimorfismo con punteros a Persona
    virtual ~Persona() {
        cout << "\n Destructor de la Clase Base" << endl;
    }

    // Metodos de acceso (getters) — lectura de atributos
    string getNombre()    const { return nombre;    }
    int    getEdad()      const { return edad;      }
    string getDomicilio() const { return domicilio; }
    string getTelefono()  const { return telefono;  }

    // Metodo virtual puro: obliga a las subclases a implementar
    // su propia representacion en texto
    virtual string escritura() const = 0;

    // Sobrecarga del operador << para sobreescribir
    friend ostream& operator<<(ostream& os, const Persona& p) {
        os << p.escritura(); // Llama al metodo polimorfico escritura()
        return os;
    }

    // Sobrecarga del operador == para comparar personas por nombre
    bool operator==(const Persona& otro) const {
        return nombre == otro.nombre; // Comparacion por nombre
    }
};

// HERENCIA PUBLICA
// CLASE DERIVADA CONCRETA: Medico (hereda de Persona)
// Agrega especialidad y horario de atencion

class Medico : public Persona {
private:
    string especialidad; // Especialidad medica (ej. Cardiologia)
    string horario;      // Horario de consulta (ej. "Lun-Vie 9-14h")
    int    id;           // Identificador unico del medico

    static int contadorId; // Variable estatica: cuenta los IDs generados

public:
    // Constructor: llama al constructor de Persona e inicializa lo propio
    Medico(const string& nombre, int edad,
           const string& domicilio, const string& telefono,
           const string& especialidad, const string& horario)
        : Persona(nombre, edad, domicilio, telefono),
          especialidad(especialidad), horario(horario)
    {
        id = ++contadorId; // Asigna el siguiente ID disponible
        cout << "\n Construccion de la Clase Derivada 1" << endl;
    }

    // Destructor
    ~Medico() {
        cout << "\n Destructor de la Clase Derivada 1" << endl;
    }

    // Getters especificos del medico
    string getEspecialidad() const { return especialidad; }
    string getHorario()      const { return horario;      }
    int    getId()           const { return id;           }

    // Implementacion obligatoria de escritura() (polimorfismo)
    string escritura() const override {
        // [Librería <sstream>] Uso de la clase ostringstream para crear un flujo temporal que formatea texto e ints
        ostringstream oss;  //clase que pertenece a sstream
        oss << "[Medico #" << this->id << "] "
            << this->nombre
            << " | Especialidad: " << this->especialidad
            << " | Horario: "      << this->horario
            << " | Tel: "          << this->telefono;

        // [Librería <sstream>] Uso del método str() para convertir el contenido del flujo a un string normal
        return oss.str();
    }
};
int Medico::contadorId = 0;

//HERENCIA PUBLICA
// CLASE DERIVADA CONCRETA: Paciente (hereda de Persona)
// Agrega un ID propio y referencia al medico que lo esta atendiendo

class Paciente : public Persona {
private:
    int    id;            // Identificador unico del paciente
    string medicoNombre;  // Nombre del medico que lo atiende
    string especialidad;  // Especialidad del medico que lo atiende

    static int contadorId; // Contador estatico de IDs

public:
    // Constructor del paciente
    Paciente(const string& nombre, int edad,
             const string& domicilio, const string& telefono,
             const string& medicoNombre, const string& especialidad)
        : Persona(nombre, edad, domicilio, telefono),
          medicoNombre(medicoNombre), especialidad(especialidad)
    {
        id = ++contadorId; // Asigna ID automaticamente
        cout << "\n Construccion de la Clase Derivada 2" << endl;
    }

    // Destructor del paciente
    ~Paciente() {
        cout << "\n Destructor de la Clase Derivada 2" << endl;
    }

    // Getters especificos del paciente
    int    getId()           const { return id;           }
    string getMedicoNombre() const { return medicoNombre; }
    string getEspecialidad() const { return especialidad; }

    // Permite actualizar el medico asignado usando 'this->'
    void setMedico(const string& nuevoMedico, const string& nuevaEsp) {
        this->medicoNombre = nuevoMedico;
        this->especialidad = nuevaEsp;
    }

    // Implementacion de escritura() para pacientes
    string escritura() const override {
        // [Librería <sstream>] ostringstream facilita juntar strings fijos, ints (edad, id) y strings dinámicos
        ostringstream oss;
        oss << "[Paciente #" << this->id << "] "
            << this->nombre
            << " | Edad: "       << this->edad
            << " | Tel: "        << this->telefono
            << " | Domicilio: "  << this->domicilio
            << " | Medico: "     << this->medicoNombre
            << " (" << this->especialidad << ")";
        return oss.str();
    }
};
int Paciente::contadorId = 0;

// CLASE CONCRETA: Cita (ES INDEPENDIENTE/CONEXION ENTRE PACIENTE-MEDICO)
// Representa una cita medica entre un paciente y un medico

class Cita {
private:
    int    id;             // ID unico de la cita
    string pacienteNombre; // Nombre del paciente
    string medicoNombre;   // Nombre del medico
    string fecha;          // Fecha en formato "YYYY-MM-DD"
    string hora;           // Hora en formato "HH:MM"
    bool   activa;         // Estado: true=vigente, false=cancelada

    static int contadorId; // Contador de IDs de citas

public:
    // Constructor de la cita
    Cita(const string& pacNombre, const string& medNombre,
         const string& fecha, const string& hora)
        : pacienteNombre(pacNombre), medicoNombre(medNombre),
          fecha(fecha), hora(hora), activa(true)
    {
        id = ++contadorId;
        cout << "\n Construccion de la Clase Cita" << endl;
    }

    // Destructor de la cita
    ~Cita() {
        cout << "\n Construccion de la Clase Cita" << endl;
    }

    // Getters de la cita
    int    getId()             const { return id;             }
    string getPacienteNombre() const { return pacienteNombre; }
    string getMedicoNombre()   const { return medicoNombre;   }
    string getFecha()          const { return fecha;          }
    string getHora()           const { return hora;           }
    bool   estaActiva()        const { return activa;         }

    // Cancela la cita cambiando su estado
    void cancelar() { activa = false; }

    // Representacion en texto de la cita
    string escritura() const {
        ostringstream oss;
        oss << "[Cita #" << id << "] "
            << fecha << " " << hora
            << " | Paciente: " << pacienteNombre
            << " | Medico: "   << medicoNombre
            << " | Estado: "   << (activa ? "Activa" : "CANCELADA");
        return oss.str();
    }

    // Sobrecarga del operador < para comparar citas por fecha y hora
    bool operator<(const Cita& otra) const {
        if (this->fecha != otra.fecha)
            return this->fecha < otra.fecha;
        return this->hora < otra.hora;
    }

    // Sobrecarga del operador == para verificar duplicados
    bool operator==(const Cita& otra) const {
        return medicoNombre   == otra.medicoNombre &&
               fecha          == otra.fecha        &&
               hora           == otra.hora         &&
               activa         == otra.activa;
    }
};
int Cita::contadorId = 0;

// ============================================================
// CLASE: GestorHospitalario (ES DE COMPOSICION/AGREGACION)
// Nucleo del sistema: administra medicos, pacientes y citas
// ============================================================
class GestorHospitalario {
private:
    Repositorio<Medico>   medicos;   // Repositorio de medicos
    Repositorio<Paciente> pacientes; // Repositorio de pacientes
    vector<Cita>          citas;     // Lista de todas las citas

    // Metodo privado: busca un medico por nombre
    Medico* buscarMedico(const string& nombre) const {
        for (Medico* m : medicos.obtenerTodos())
            if (m->getNombre() == nombre)
                return m;
        return nullptr; // Por default devuelve un valor nulo de que no encontro al doctor
    }

    // Metodo privado: busca un paciente por nombre
    Paciente* buscarPaciente(const string& nombre) const {
        for (Paciente* p : pacientes.obtenerTodos())
            if (p->getNombre() == nombre)
                return p;
        return nullptr;
    }

    // Metodo privado: verifica si un medico esta disponible
    bool medicoDisponible(const string& medNombre,
                          const string& fecha,
                          const string& hora) const {
        for (const Cita& c : citas) {
            if (c.getMedicoNombre() == medNombre &&
                c.getFecha()        == fecha     &&
                c.getHora()         == hora      &&
                c.estaActiva())
            {
                return false;
            }
        }
        return true;
    }

    // Metodo privado: lee un campo "Etiqueta : valor"
    string leerCampo(const string& linea) const {
        // [Librería <string>] Uso de find() para buscar un caracter dentro del string. Retorna su posición.
        size_t pos = linea.find(':');

        // [Librería <string>] Uso de string::npos que representa el valor "no encontrado" al buscar en una cadena.
        if (pos == string::npos) return "";

        // [Librería <string>] Uso de substr() para extraer una parte de la cadena, desde (pos+1) en adelante.
        string valor = linea.substr(pos + 1);

        // [Librería <string>] Uso de find_first_not_of() para encontrar el primer carácter que no sea espacio o tabulación.
        size_t inicio = valor.find_first_not_of(" \t");
        if (inicio == string::npos) return "";
        return valor.substr(inicio);
    }

    // Metodo privado: verifica si un medico ya existe
    bool medicoYaExiste(const string& nombre) const {
        for (Medico* m : medicos.obtenerTodos())
            if (m->getNombre() == nombre)
                return true;
        return false;
    }

    // Metodo privado: verifica si un paciente ya existe
    bool pacienteYaExiste(const string& nombre) const {
        for (Paciente* p : pacientes.obtenerTodos())
            if (p->getNombre() == nombre)
                return true;
        return false;
    }

public:
    GestorHospitalario() {
        cout << "\n Construccion de la Clase Gestor Hospitalario" << endl;
    }

    ~GestorHospitalario() {
        cout << "\n Destructor de la Clase Gestor Hospitalario" << endl;
    }

    // CARGA DE DATOS PRECARGADOS
    //Instanciaciones de la plantilla
    void cargarDatosPrecargados() {
        //agregar es un metodo de la plantilla
        //Cuando se llam a este metodo se hace uso de la funcion push_back
        //new Medico(...): Se crea un objeto médico en la memoria dinámica
        medicos.agregar(new Medico("Dr. Carlos Ramirez", 45, "Av. Morelos, EDOMEX", "5551234567", "Cardiologia", "Lun-Vie 08:00-14:00"));
        medicos.agregar(new Medico("Dra. Laura Mendoza", 38, "Calle Roble 22, Queretaro", "4421234567", "Pediatria", "Lun-Mie-Vie 10:00-16:00"));
        medicos.agregar(new Medico("Dr. Jorge Herrera", 52, "Blvd. Zaragoza, Ciudad Juarez", "6560971294", "Neurologia", "Mar-Jue 09:00-15:00"));
        medicos.agregar(new Medico("Dra. Ana Torres", 41, "Calle Cedros 8, Leon", "4771234567", "Dermatologia", "Lun-Vie 11:00-17:00"));
        medicos.agregar(new Medico("Dr. Miguel Soto", 55, "Av. Independencia, Ciudad Juarez", "6579876543", "Traumatologia", "Lun-Vie 07:00-13:00"));

        pacientes.agregar(new Paciente("Maria Lopez", 34, "Av Morelos 5, EDOMEX", "5643619527", "Dr. Carlos Ramirez", "Cardiologia"));
        pacientes.agregar(new Paciente("Juan Garcia", 8, "Av. Zaragoza 12, Queretaro", "4427381750", "Dra. Laura Mendoza", "Pediatria"));
        pacientes.agregar(new Paciente("Rosa Perez", 62, "Av Enrique Pinocelli, Ciudad Juarez", "6568730176", "Dr. Jorge Herrera", "Neurologia"));
        pacientes.agregar(new Paciente("Luis Hernandez", 29, "Blvd. Bernardo Quintana 10, Veracruz", "2297361964", "Dra. Ana Torres", "Dermatologia"));
        pacientes.agregar(new Paciente("Sofia Martinez", 45, "Av. 5 de Febrero 88, Irapuato", "4625519844", "Dr. Miguel Soto", "Traumatologia"));
        pacientes.agregar(new Paciente("Carlos Jimenez", 17, "Calle Laurel 7, Guadalajara", "3346678612", "Dra. Laura Mendoza", "Pediatria"));
        pacientes.agregar(new Paciente("Ana Morales", 55, "Calle El Remolino 4, Veracruz", "2297819401", "Dr. Carlos Ramirez", "Cardiologia"));
        pacientes.agregar(new Paciente("Pedro Gonzalez", 38, "Av. San Miguel 50, EDOMEX", "5591873094", "Dr. Jorge Herrera", "Neurologia"));
        pacientes.agregar(new Paciente("Lucia Ramirez", 23, "Av Reforma, CDMX", "5624509431", "Dra. Ana Torres", "Dermatologia"));
        pacientes.agregar(new Paciente("Fernando Cruz", 71, "Blvd. Aeropuerto 200, Celaya", "4610199110", "Dr. Miguel Soto", "Traumatologia"));
        pacientes.agregar(new Paciente("Elena Vargas", 5, "Calle Puerto de Palos, Ciudad Juarez", "6561096778", "Dra. Laura Mendoza", "Pediatria"));
        pacientes.agregar(new Paciente("Andres Reyes", 49, "Av. Universidad 30, Poza Rica", "7829146910", "Dr. Carlos Ramirez", "Cardiologia"));
        pacientes.agregar(new Paciente("Patricia Salinas", 33, "Calle Cipres 15, Saltillo", "4449186910", "Dr. Jorge Herrera", "Neurologia"));
        pacientes.agregar(new Paciente("Roberto Castillo", 60, "Av. Tecnologico 100, San Luis", "4441094101", "Dra. Ana Torres", "Dermatologia"));
        pacientes.agregar(new Paciente("Claudia Ramos", 42, "Calle Sauce 2, Tampico", "8339827512", "Dr. Miguel Soto", "Traumatologia"));

        cout << "\n[Sistema] Datos precargados: "
        //cantidad es un metodo de la plantilla para mostrar todos los elementos del vector correspondiente
             << medicos.cantidad()   << " medicos, "
             << pacientes.cantidad() << " pacientes.\n";
    }

    void cargarMedicosDesdeTxt(const string& nombreArchivo) {
        // [Librería <fstream>] Uso de ifstream (input file stream) para leer un archivo desde el disco duro
        ifstream archivo(nombreArchivo);

        // [Librería <fstream>] is_open() comprueba si se logró abrir el archivo de manera exitosa
        if (!archivo.is_open()) {
            cout << "[Info] Archivo '" << nombreArchivo
                 << "' no encontrado. Se usaran solo los datos precargados.\n"; //En caso de que aun no este creado
            return;
        }

        string linea;
        string nombre = "", especialidad = "", horario = "", telefono = "";
        int nuevos = 0;

        // [Librería <string>] Uso de getline para extraer línea por línea desde el archivo hacia el string 'linea'
        while (getline(archivo, linea)) {

            if      (linea.find("Nombre       :") != string::npos)
                nombre       = leerCampo(linea);
            else if (linea.find("Especialidad :") != string::npos)
                especialidad = leerCampo(linea);
            else if (linea.find("Horario      :") != string::npos)
                horario      = leerCampo(linea);
            else if (linea.find("Telefono     :") != string::npos)
                telefono     = leerCampo(linea);

            else if (linea.find("----") != string::npos) {
                if (!nombre.empty() && !medicoYaExiste(nombre)) {
                    medicos.agregar(new Medico(nombre, 0, "", telefono,
                                               especialidad, horario));
                    nuevos++;
                }
                nombre = especialidad = horario = telefono = "";
            }
        }

        // [Librería <fstream>] Uso de close() para liberar los recursos del archivo al finalizar
        archivo.close();
        cout << "[Info] Medicos cargados desde '" << nombreArchivo
        //Informar cuantos medicos nuevos se cargaron desde el archivo
             << "': " << nuevos << " nuevo(s).\n";
    }

    void cargarPacientesDesdeTxt(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "[Info] Archivo '" << nombreArchivo
                 << "' no encontrado. Se usaran solo los datos precargados.\n";
            return;
        }
        string linea;
        string nombre = "", domicilio = "", telefono = "";
        string medicoNom = "", especialidad = "";
        int edad = 0;
        int nuevos = 0;

        while (getline(archivo, linea)) {

            if      (linea.find("Nombre       :") != string::npos)
                nombre       = leerCampo(linea);
            else if (linea.find("Edad         :") != string::npos)
                // [Librería <string>] Uso de stoi() (string to int) para convertir la cadena leída a un valor entero
                edad         = stoi(leerCampo(linea));
            else if (linea.find("Domicilio    :") != string::npos)
                domicilio    = leerCampo(linea);
            else if (linea.find("Telefono     :") != string::npos)
                telefono     = leerCampo(linea);
            else if (linea.find("Medico       :") != string::npos)
                medicoNom    = leerCampo(linea);
            else if (linea.find("Especialidad :") != string::npos)
                especialidad = leerCampo(linea);

            else if (linea.find("----") != string::npos) {
                if (!nombre.empty() && !pacienteYaExiste(nombre)) {
                    pacientes.agregar(new Paciente(nombre, edad, domicilio,
                                                   telefono, medicoNom,
                                                   especialidad));
                    nuevos++;
                }
                nombre = domicilio = telefono = medicoNom = especialidad = "";
                edad = 0;
            }
        }
        archivo.close();
        cout << "[Info] Pacientes cargados desde '" << nombreArchivo
             << "': " << nuevos << " nuevo(s).\n";
    }

    void registrarMedico() {
        string nombre, domicilio, telefono, especialidad, horario;
        int edad;

        cout << "\n--- Registrar Nuevo Medico ---\n";

        // [Librería <iostream> / <string>] ignore() limpia saltos de línea pendientes en el buffer antes de usar getline()
        cout << "Nombre completo: "; cin.ignore(); getline(cin, nombre);
        cout << "Edad: ";           cin >> edad; cin.ignore();
        cout << "Domicilio: ";      getline(cin, domicilio);
        cout << "Telefono: ";       getline(cin, telefono);
        cout << "Especialidad: ";   getline(cin, especialidad);
        cout << "Horario (ej. Lun-Vie 09:00-15:00): "; getline(cin, horario);

        medicos.agregar(new Medico(nombre, edad, domicilio, telefono,
                                   especialidad, horario));
        cout << "[OK] Medico registrado exitosamente.\n";
    }

    void registrarPaciente() {
        string nombre, domicilio, telefono, medNombre, esp;
        int edad;

        cout << "\n--- Registrar Nuevo Paciente ---\n";
        cout << "Nombre completo: "; cin.ignore(); getline(cin, nombre);
        cout << "Edad: ";            cin >> edad; cin.ignore();
        cout << "Domicilio: ";       getline(cin, domicilio);
        cout << "Telefono: ";        getline(cin, telefono);

        listarMedicos();
        cout << "Nombre del medico asignado: "; getline(cin, medNombre);

        Medico* m = buscarMedico(medNombre);
        if (!m) {
            cout << "[Error] Medico no encontrado. Paciente sin medico asignado.\n";
            esp = "N/A";
            medNombre = "Sin asignar";
        } else {
            esp = m->getEspecialidad();
        }

        pacientes.agregar(new Paciente(nombre, edad, domicilio, telefono,
                                       medNombre, esp));
        cout << "[OK] Paciente registrado exitosamente.\n";
    }

    void programarCita() {
        string pacNombre, medNombre, fecha, hora;

        cout << "\n--- Programar Cita ---\n";
        cin.ignore();
        cout << "Nombre del paciente: "; getline(cin, pacNombre);
        cout << "Nombre del medico: ";   getline(cin, medNombre);
        cout << "Fecha (YYYY-MM-DD): ";  getline(cin, fecha);
        cout << "Hora  (HH:MM): ";       getline(cin, hora);

        if (!buscarPaciente(pacNombre)) {
            cout << "[Error] Paciente no encontrado.\n"; return;
        }
        if (!buscarMedico(medNombre)) {
            cout << "[Error] Medico no encontrado.\n"; return;
        }
        if (!medicoDisponible(medNombre, fecha, hora)) {
            cout << "[Error] El medico ya tiene una cita en ese horario.\n"; return;
        }

        // [Librería <vector>] Uso de emplace_back() para construir el objeto directamente al final del vector, ahorrando memoria
        citas.emplace_back(pacNombre, medNombre, fecha, hora);
        cout << "[OK] Cita programada exitosamente.\n";
    }

    void cancelarCita() {
        int idBuscar;
        cout << "\n--- Cancelar Cita ---\n";
        cout << "ID de la cita a cancelar: "; cin >> idBuscar;

        for (Cita& c : citas) {
            if (c.getId() == idBuscar) {
                if (!c.estaActiva()) {
                    cout << "[Aviso] La cita ya estaba cancelada.\n";
                } else {
                    c.cancelar();
                    cout << "[OK] Cita cancelada.\n";
                }
                return;
            }
        }
        cout << "[Error] Cita no encontrada.\n";
    }

    void consultarCitasPaciente() const {
        string nombre;
        cin.ignore();
        cout << "\n--- Citas por Paciente ---\n";
        cout << "Nombre del paciente: "; getline(cin, nombre);

        bool encontro = false;
        for (const Cita& c : citas) {
            if (c.getPacienteNombre() == nombre) {
                cout << c.escritura() << "\n";
                encontro = true;
            }
        }
        if (!encontro)
            cout << "[Info] El paciente no tiene citas registradas.\n";
    }

    void consultarCitasMedico() const {
        string nombre;
        cin.ignore();
        cout << "\n--- Citas por Medico (ordenadas por fecha) ---\n";
        cout << "Nombre del medico: "; getline(cin, nombre);

        vector<Cita> citasMedico;
        for (const Cita& c : citas) {
            if (c.getMedicoNombre() == nombre)
                citasMedico.push_back(c);
        }

        // [Librería <vector>] Uso de empty() para comprobar de forma rápida si el vector no contiene elementos
        if (citasMedico.empty()) {
            cout << "[Info] El medico no tiene citas registradas.\n"; return;
        }

        // [Librería <algorithm>] Uso de sort() para ordenar las citas. Utiliza implicitamente el "operator<" que se sobrecargo en Cita.
        // [Librería <vector>] Uso de begin() y end() para indicarle a sort() el rango de memoria que debe ordenar.
        sort(citasMedico.begin(), citasMedico.end());

        for (const Cita& c : citasMedico)
            cout << c.escritura() << "\n";
    }

    void consultarHistorialPaciente() const {
        string nombre;
        cin.ignore();
        cout << "\n--- Historial de Paciente ---\n";
        cout << "Nombre del paciente: "; getline(cin, nombre);

        Paciente* p = buscarPaciente(nombre);
        if (!p) { cout << "[Error] Paciente no encontrado.\n"; return; }

        cout << "\nDatos del paciente:\n" << *p << "\n";

        cout << "\nHistorial de citas:\n";
        bool tieneCitas = false;
        for (const Cita& c : citas) {
            if (c.getPacienteNombre() == nombre) {
                cout << "  " << c.escritura() << "\n";
                tieneCitas = true;
            }
        }
        if (!tieneCitas)
            cout << "  (Sin citas registradas)\n";
    }

    void listarMedicos() const {
        cout << "\n--- Lista de Medicos ---\n";
        for (Medico* m : medicos.obtenerTodos())
            cout << *m << "\n";
    }

    void listarPacientes() const {
        cout << "\n--- Lista de Pacientes ---\n";
        for (Paciente* p : pacientes.obtenerTodos())
            cout << *p << "\n";
    }

    void exportarMedicos() const {
        string nombreArchivo;
        cin.ignore();
        cout << "Nombre del archivo (sin extension): "; getline(cin, nombreArchivo);
        nombreArchivo += ".txt";

        // [Librería <fstream>] Uso de ofstream (output file stream) para crear y abrir un archivo de escritura
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "[Error] No se pudo crear el archivo.\n"; return;
        }

        // [Librería <fstream>] El operador << se usa para agregar texto directamente hacia el archivo
        archivo << "====================================\n";
        archivo << "  LISTADO DE MEDICOS - HOSPITAL\n";
        archivo << "====================================\n\n";

        for (Medico* m : medicos.obtenerTodos()) {
            archivo << "Nombre       : " << m->getNombre()       << "\n";
            archivo << "Especialidad : " << m->getEspecialidad() << "\n";
            archivo << "Horario      : " << m->getHorario()      << "\n";
            archivo << "Telefono     : " << m->getTelefono()     << "\n";
            archivo << "------------------------------------\n";
        }

        archivo.close();
        cout << "[OK] Archivo '" << nombreArchivo << "' generado.\n";
    }

    void exportarPacientes() const {
        string nombreArchivo;
        cin.ignore();
        cout << "Nombre del archivo (sin extension): "; getline(cin, nombreArchivo);
        nombreArchivo += ".txt";

        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "[Error] No se pudo crear el archivo.\n"; return;
        }

        archivo << "====================================\n";
        archivo << "  LISTADO DE PACIENTES - HOSPITAL\n";
        archivo << "====================================\n\n";

        for (Paciente* p : pacientes.obtenerTodos()) {
            archivo << "Nombre       : " << p->getNombre()       << "\n";
            archivo << "Edad         : " << p->getEdad()         << "\n";
            archivo << "Domicilio    : " << p->getDomicilio()    << "\n";
            archivo << "Telefono     : " << p->getTelefono()     << "\n";
            archivo << "Medico       : " << p->getMedicoNombre() << "\n";
            archivo << "Especialidad : " << p->getEspecialidad() << "\n";
            archivo << "------------------------------------\n";
        }

        archivo.close();
        cout << "[OK] Archivo '" << nombreArchivo << "' generado.\n";
    }
};

// ============================================================
// MENU PRINCIPAL DEL SISTEMA
// ============================================================
void mostrarMenu() {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout <<   "║   SISTEMA DE GESTION HOSPITALARIA    ║\n";
    cout <<   "╠══════════════════════════════════════╣\n";
    cout <<   "║  1. Registrar medico                 ║\n";
    cout <<   "║  2. Registrar paciente               ║\n";
    cout <<   "║  3. Programar cita                   ║\n";
    cout <<   "║  4. Cancelar cita                    ║\n";
    cout <<   "║  5. Ver citas de un paciente         ║\n";
    cout <<   "║  6. Ver citas de un medico           ║\n";
    cout <<   "║  7. Historial de paciente            ║\n";
    cout <<   "║  8. Listar medicos                   ║\n";
    cout <<   "║  9. Listar pacientes                 ║\n";
    cout <<   "║ 10. Exportar medicos a .txt          ║\n";
    cout <<   "║ 11. Exportar pacientes a .txt        ║\n";
    cout <<   "║  0. Salir                            ║\n";
    cout <<   "╚══════════════════════════════════════╝\n";
    cout <<   "Opcion: ";
}

// ============================================================
// FUNCION PRINCIPAL (main)
// ============================================================
int main() {
    GestorHospitalario gestor;
    gestor.cargarDatosPrecargados();

    gestor.cargarMedicosDesdeTxt("Lista_de_Medicos.txt");
    gestor.cargarPacientesDesdeTxt("Lista_de_Pacientes.txt");

    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;

        if (cin.fail()) {
            // [Librería <iostream>] clear() reinicia el estado de error de 'cin' (cuando el usuario ingresa letras en vez de int)
            cin.clear();

            // [Librería <limits>] Uso de numeric_limits::max() para decirle al compilador "ignora hasta el número máximo posible de caracteres en memoria"
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = -1;
        }

        switch (opcion) {
            case 1:  gestor.registrarMedico();           break;
            case 2:  gestor.registrarPaciente();         break;
            case 3:  gestor.programarCita();             break;
            case 4:  gestor.cancelarCita();              break;
            case 5:  gestor.consultarCitasPaciente();    break;
            case 6:  gestor.consultarCitasMedico();      break;
            case 7:  gestor.consultarHistorialPaciente();break;
            case 8:  gestor.listarMedicos();             break;
            case 9:  gestor.listarPacientes();           break;
            case 10: gestor.exportarMedicos();           break;
            case 11: gestor.exportarPacientes();         break;
            case 0:  cout << "\n[Sistema] Cerrando el sistema. Hasta pronto.\n"; break;
            default: cout << "[Aviso] Opcion no valida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}
