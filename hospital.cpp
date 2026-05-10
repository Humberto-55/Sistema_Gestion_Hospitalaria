/*
 * ============================================================
 *  SISTEMA DE GESTION HOSPITALARIA
 *  Materia: Programacion Orientada a Objetos
 *  Autor: Humberto Hernandez Nonigo
 *  Descripcion: Administracion de pacientes, medicos y citas
 * ============================================================
 */

#include <iostream>   // Entrada/salida estandar
#include <vector>     // Contenedor dinamico para listas
#include <string>     // Manejo de cadenas
#include <algorithm>  // Para ordenar (sort)
#include <fstream>    // Para escritura de archivos
#include <sstream>    // Para construir cadenas con formato
#include <limits>     // Para limpiar buffer de entrada

using namespace std;

// ============================================================
// PLANTILLA (TEMPLATE): Repositorio que permite almacenar cualquier tipo T en un vector dinamico
// ============================================================
template <typename T>
class Repositorio {
private:
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
        elementos.clear();      // Vacia el vector
        cout <<"Destruccion de la Plantilla" << endl;
    }

    // Agrega un nuevo elemento al repositorio
    void agregar(T* elem) {
        elementos.push_back(elem); // Inserta al final del vector
    }

    // Devuelve el vector completo de elementos (solo lectura)
    const vector<T*>& obtenerTodos() const {
        return elementos;
    }

    // Devuelve el numero de elementos almacenados
    int cantidad() const {
        return (int)elementos.size();
    }
};

// CLASE BASE: Persona
// Contiene los atributos comunes de medicos y pacientes

class Persona {
protected:
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

    // Sobrecarga del operador <<
    // Permite imprimir un objeto Persona directamente con cout
    friend ostream& operator<<(ostream& os, const Persona& p) {
        os << p.escritura(); // Llama al metodo polimorfico escritura()
        return os;
    }

    // Sobrecarga del operador == para comparar personas por nombre
    bool operator==(const Persona& otro) const {
        return nombre == otro.nombre; // Comparacion por nombre
    }
};

// CLASE DERIVADA: Medico (hereda de Persona)
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

    // Destructor: mensaje para verificar destruccion (uso del destructor)
    ~Medico() {
        // El destructor libera recursos; aqui solo se indica su ejecucion
        cout << "\n Destructor de la Clase Derivada 1" << endl;
    }

    // Getters especificos del medico
    string getEspecialidad() const { return especialidad; }
    string getHorario()      const { return horario;      }
    int    getId()           const { return id;           }

    // Implementacion obligatoria de escritura() (polimorfismo)
    string escritura() const override {
        // Construye la representacion en texto usando 'this->'
        // (uso explicito del puntero this)
        //herramientas de sstream
        ostringstream oss;
        oss << "[Medico #" << this->id << "] "
            << this->nombre
            << " | Especialidad: " << this->especialidad
            << " | Horario: "      << this->horario
            << " | Tel: "          << this->telefono;
        return oss.str();
    }
};

// Inicializacion del contador estatico fuera de la clase
int Medico::contadorId = 0;

// ============================================================
// CLASE DERIVADA: Paciente (hereda de Persona)
// Agrega un ID propio y referencia al medico tratante
// ============================================================
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
        this->medicoNombre = nuevoMedico; // 'this->' accede al atributo propio
        this->especialidad = nuevaEsp;
    }

    // Implementacion de escritura() para pacientes
    string escritura() const override {
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

// Inicializacion del contador estatico de Paciente
int Paciente::contadorId = 0;

// ============================================================
// CLASE: Cita
// Representa una cita medica entre un paciente y un medico
// ============================================================
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
        id = ++contadorId; // Asigna el siguiente ID
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
    // Permite ordenarlas cronologicamente con std::sort
    bool operator<(const Cita& otra) const {
        if (this->fecha != otra.fecha)
            return this->fecha < otra.fecha; // Primero compara fecha
        return this->hora < otra.hora;       // Si igual fecha, compara hora
    }

    // Sobrecarga del operador == para verificar duplicados
    bool operator==(const Cita& otra) const {
        return medicoNombre   == otra.medicoNombre &&
               fecha          == otra.fecha        &&
               hora           == otra.hora         &&
               activa         == otra.activa;
    }
};

// Inicializacion del contador de Cita
int Cita::contadorId = 0;

// ============================================================
// CLASE: GestorHospitalario
// Nucleo del sistema: administra medicos, pacientes y citas
// ============================================================
class GestorHospitalario {
private:
    Repositorio<Medico>   medicos;   // Repositorio de medicos
    Repositorio<Paciente> pacientes; // Repositorio de pacientes
    vector<Cita>          citas;     // Lista de todas las citas

    // -------------------------------------------------------
    // Metodo privado: busca un medico por nombre
    // Devuelve puntero al medico o nullptr si no existe
    // -------------------------------------------------------
    Medico* buscarMedico(const string& nombre) const {
        for (Medico* m : medicos.obtenerTodos()) // Itera sobre medicos
            if (m->getNombre() == nombre)         // Compara nombres
                return m;                         // Retorna si encuentra
        return nullptr; // No encontrado
    }

    // -------------------------------------------------------
    // Metodo privado: busca un paciente por nombre
    // -------------------------------------------------------
    Paciente* buscarPaciente(const string& nombre) const {
        for (Paciente* p : pacientes.obtenerTodos())
            if (p->getNombre() == nombre)
                return p;
        return nullptr;
    }

    // -------------------------------------------------------
    // Metodo privado: verifica si un medico esta disponible
    // en la fecha y hora indicadas (sin otra cita activa)
    // -------------------------------------------------------
    bool medicoDisponible(const string& medNombre,
                          const string& fecha,
                          const string& hora) const {
        for (const Cita& c : citas) {           // Recorre todas las citas
            if (c.getMedicoNombre() == medNombre &&
                c.getFecha()        == fecha     &&
                c.getHora()         == hora      &&
                c.estaActiva())                  // Solo citas activas
            {
                return false; // El medico ya tiene cita en ese horario
            }
        }
        return true; // El medico esta libre
    }

public:
    // Constructor del gestor (vacio; los datos se cargan aparte)
    GestorHospitalario() {
        cout << "\n Construccion de la Clase Gestor Hospitalario" << endl;
    }

    // Destructor del gestor
    ~GestorHospitalario() {
        cout << "\n Destructor de la Clase Gestor Hospitalario" << endl;
    }

    // -------------------------------------------------------
    // CARGA DE DATOS PRECARGADOS
    // Registra 5 medicos y 15 pacientes iniciales
    // -------------------------------------------------------
    void cargarDatosPrecargados() {

        // --- 5 Medicos ---
        medicos.agregar(new Medico(
            "Dr. Carlos Ramirez", 45, "Av. Morelos, EDOMEX", "5551234567",
            "Cardiologia", "Lun-Vie 08:00-14:00"));

        medicos.agregar(new Medico(
            "Dra. Laura Mendoza", 38, "Calle Roble 22, Queretaro", "4421234567",
            "Pediatria", "Lun-Mie-Vie 10:00-16:00"));

        medicos.agregar(new Medico(
            "Dr. Jorge Herrera", 52, "Blvd. Zaragoza, Ciudad Juarez", "6560971294",
            "Neurologia", "Mar-Jue 09:00-15:00"));

        medicos.agregar(new Medico(
            "Dra. Ana Torres", 41, "Calle Cedros 8, Leon", "4771234567",
            "Dermatologia", "Lun-Vie 11:00-17:00"));

        medicos.agregar(new Medico(
            "Dr. Miguel Soto", 55, "Av. Independencia, Ciudad Juarez", "6579876543",
            "Traumatologia", "Lun-Vie 07:00-13:00"));

        // --- 15 Pacientes ---
        pacientes.agregar(new Paciente(
            "Maria Lopez", 34, "Av Morelos 5, EDOMEX", "5643619527",
            "Dr. Carlos Ramirez", "Cardiologia"));

        pacientes.agregar(new Paciente(
            "Juan Garcia", 8, "Av. Zaragoza 12, Queretaro", "4427381750",
            "Dra. Laura Mendoza", "Pediatria"));

        pacientes.agregar(new Paciente(
            "Rosa Perez", 62, "Av Enrique Pinocelli, Ciudad Juarez", "6568730176",
            "Dr. Jorge Herrera", "Neurologia"));

        pacientes.agregar(new Paciente(
            "Luis Hernandez", 29, "Blvd. Bernardo Quintana 10, Veracruz", "2297361964",
            "Dra. Ana Torres", "Dermatologia"));

        pacientes.agregar(new Paciente(
            "Sofia Martinez", 45, "Av. 5 de Febrero 88, Irapuato", "4625519844",
            "Dr. Miguel Soto", "Traumatologia"));

        pacientes.agregar(new Paciente(
            "Carlos Jimenez", 17, "Calle Laurel 7, Guadalajara", "3346678612",
            "Dra. Laura Mendoza", "Pediatria"));

        pacientes.agregar(new Paciente(
            "Ana Morales", 55, "Calle El Remolino 4, Veracruz", "2297819401",
            "Dr. Carlos Ramirez", "Cardiologia"));

        pacientes.agregar(new Paciente(
            "Pedro Gonzalez", 38, "Av. San Miguel 50, EDOMEX", "5591873094",
            "Dr. Jorge Herrera", "Neurologia"));

        pacientes.agregar(new Paciente(
            "Lucia Ramirez", 23, "Av Reforma, CDMX", "5624509431",
            "Dra. Ana Torres", "Dermatologia"));

        pacientes.agregar(new Paciente(
            "Fernando Cruz", 71, "Blvd. Aeropuerto 200, Celaya", "4610199110",
            "Dr. Miguel Soto", "Traumatologia"));

        pacientes.agregar(new Paciente(
            "Elena Vargas", 5, "Calle Puerto de Palos, Ciudad Juarez", "6561096778",
            "Dra. Laura Mendoza", "Pediatria"));

        pacientes.agregar(new Paciente(
            "Andres Reyes", 49, "Av. Universidad 30, Poza Rica", "7829146910",
            "Dr. Carlos Ramirez", "Cardiologia"));

        pacientes.agregar(new Paciente(
            "Patricia Salinas", 33, "Calle Cipres 15, Saltillo", "4449186910",
            "Dr. Jorge Herrera", "Neurologia"));

        pacientes.agregar(new Paciente(
            "Roberto Castillo", 60, "Av. Tecnologico 100, San Luis", "4441094101",
            "Dra. Ana Torres", "Dermatologia"));

        pacientes.agregar(new Paciente(
            "Claudia Ramos", 42, "Calle Sauce 2, Tampico", "8339827512",
            "Dr. Miguel Soto", "Traumatologia"));

        cout << "\n[Sistema] Datos precargados: "
             << medicos.cantidad()   << " medicos, "
             << pacientes.cantidad() << " pacientes.\n";
    }

    // -------------------------------------------------------
    // REGISTRAR MEDICO desde consola
    // -------------------------------------------------------
    void registrarMedico() {
        string nombre, domicilio, telefono, especialidad, horario;
        int edad;

        cout << "\n--- Registrar Nuevo Medico ---\n";
        cout << "Nombre completo: "; cin.ignore(); getline(cin, nombre);
        cout << "Edad: ";           cin >> edad; cin.ignore();
        cout << "Domicilio: ";      getline(cin, domicilio);
        cout << "Telefono: ";       getline(cin, telefono);
        cout << "Especialidad: ";   getline(cin, especialidad);
        cout << "Horario (ej. Lun-Vie 09:00-15:00): "; getline(cin, horario);

        // Crea dinamicamente el objeto Medico y lo agrega al repositorio
        medicos.agregar(new Medico(nombre, edad, domicilio, telefono,
                                   especialidad, horario));
        cout << "[OK] Medico registrado exitosamente.\n";
    }

    // -------------------------------------------------------
    // REGISTRAR PACIENTE desde consola
    // -------------------------------------------------------
    void registrarPaciente() {
        string nombre, domicilio, telefono, medNombre, esp;
        int edad;

        cout << "\n--- Registrar Nuevo Paciente ---\n";
        cout << "Nombre completo: "; cin.ignore(); getline(cin, nombre);
        cout << "Edad: ";            cin >> edad; cin.ignore();
        cout << "Domicilio: ";       getline(cin, domicilio);
        cout << "Telefono: ";        getline(cin, telefono);

        // Muestra medicos disponibles para asignar
        listarMedicos();
        cout << "Nombre del medico asignado: "; getline(cin, medNombre);

        Medico* m = buscarMedico(medNombre); // Verifica que el medico exista
        if (!m) {
            cout << "[Error] Medico no encontrado. Paciente sin medico asignado.\n";
            esp = "N/A";
            medNombre = "Sin asignar";
        } else {
            esp = m->getEspecialidad(); // Obtiene la especialidad automaticamente
        }

        pacientes.agregar(new Paciente(nombre, edad, domicilio, telefono,
                                       medNombre, esp));
        cout << "[OK] Paciente registrado exitosamente.\n";
    }

    // -------------------------------------------------------
    // PROGRAMAR CITA
    // -------------------------------------------------------
    void programarCita() {
        string pacNombre, medNombre, fecha, hora;

        cout << "\n--- Programar Cita ---\n";
        cin.ignore();
        cout << "Nombre del paciente: "; getline(cin, pacNombre);
        cout << "Nombre del medico: ";   getline(cin, medNombre);
        cout << "Fecha (YYYY-MM-DD): ";  getline(cin, fecha);
        cout << "Hora  (HH:MM): ";       getline(cin, hora);

        // Verifica que el paciente exista en el sistema
        if (!buscarPaciente(pacNombre)) {
            cout << "[Error] Paciente no encontrado.\n"; return;
        }
        // Verifica que el medico exista en el sistema
        if (!buscarMedico(medNombre)) {
            cout << "[Error] Medico no encontrado.\n"; return;
        }
        // Verifica disponibilidad del medico en fecha/hora solicitada
        if (!medicoDisponible(medNombre, fecha, hora)) {
            cout << "[Error] El medico ya tiene una cita en ese horario.\n"; return;
        }

        // Crea la cita y la agrega a la lista
        citas.emplace_back(pacNombre, medNombre, fecha, hora);
        cout << "[OK] Cita programada exitosamente.\n";
    }

    // -------------------------------------------------------
    // CANCELAR CITA por ID
    // -------------------------------------------------------
    void cancelarCita() {
        int idBuscar;
        cout << "\n--- Cancelar Cita ---\n";
        cout << "ID de la cita a cancelar: "; cin >> idBuscar;

        for (Cita& c : citas) { // Busca la cita por ID
            if (c.getId() == idBuscar) {
                if (!c.estaActiva()) {
                    cout << "[Aviso] La cita ya estaba cancelada.\n";
                } else {
                    c.cancelar(); // Marca la cita como cancelada
                    cout << "[OK] Cita cancelada.\n";
                }
                return;
            }
        }
        cout << "[Error] Cita no encontrada.\n";
    }

    // -------------------------------------------------------
    // CONSULTAR CITAS DE UN PACIENTE
    // -------------------------------------------------------
    void consultarCitasPaciente() const {
        string nombre;
        cin.ignore();
        cout << "\n--- Citas por Paciente ---\n";
        cout << "Nombre del paciente: "; getline(cin, nombre);

        bool encontro = false;
        for (const Cita& c : citas) { // Recorre todas las citas
            if (c.getPacienteNombre() == nombre) {
                cout << c.escritura() << "\n"; // Imprime la cita
                encontro = true;
            }
        }
        if (!encontro)
            cout << "[Info] El paciente no tiene citas registradas.\n";
    }

    // -------------------------------------------------------
    // CONSULTAR CITAS DE UN MEDICO (ordenadas por fecha)
    // -------------------------------------------------------
    void consultarCitasMedico() const {
        string nombre;
        cin.ignore();
        cout << "\n--- Citas por Medico (ordenadas por fecha) ---\n";
        cout << "Nombre del medico: "; getline(cin, nombre);

        // Extrae las citas del medico seleccionado
        vector<Cita> citasMedico;
        for (const Cita& c : citas) {
            if (c.getMedicoNombre() == nombre)
                citasMedico.push_back(c); // Copia la cita al subvector
        }

        if (citasMedico.empty()) {
            cout << "[Info] El medico no tiene citas registradas.\n"; return;
        }

        // Ordena las citas usando el operador< sobrecargado en Cita
        sort(citasMedico.begin(), citasMedico.end());

        for (const Cita& c : citasMedico)
            cout << c.escritura() << "\n"; // Imprime cada cita ordenada
    }

    // -------------------------------------------------------
    // CONSULTAR HISTORIAL DE UN PACIENTE
    // -------------------------------------------------------
    void consultarHistorialPaciente() const {
        string nombre;
        cin.ignore();
        cout << "\n--- Historial de Paciente ---\n";
        cout << "Nombre del paciente: "; getline(cin, nombre);

        Paciente* p = buscarPaciente(nombre);
        if (!p) { cout << "[Error] Paciente no encontrado.\n"; return; }

        // Imprime datos del paciente usando el operador<< sobrecargado
        cout << "\nDatos del paciente:\n" << *p << "\n";

        // Muestra todas sus citas (historial completo)
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

    // -------------------------------------------------------
    // LISTAR TODOS LOS MEDICOS en consola
    // -------------------------------------------------------
    void listarMedicos() const {
        cout << "\n--- Lista de Medicos ---\n";
        for (Medico* m : medicos.obtenerTodos())
            cout << *m << "\n"; // Usa el operador<< heredado de Persona
    }

    // -------------------------------------------------------
    // LISTAR TODOS LOS PACIENTES en consola
    // -------------------------------------------------------
    void listarPacientes() const {
        cout << "\n--- Lista de Pacientes ---\n";
        for (Paciente* p : pacientes.obtenerTodos())
            cout << *p << "\n"; // Usa el operador<< heredado de Persona
    }

    // -------------------------------------------------------
    // EXPORTAR MEDICOS A ARCHIVO .TXT
    // -------------------------------------------------------
    void exportarMedicos() const {
        string nombreArchivo;
        cin.ignore();
        cout << "Nombre del archivo (sin extension): "; getline(cin, nombreArchivo);
        nombreArchivo += ".txt"; // Agrega la extension automaticamente

        ofstream archivo(nombreArchivo); // Abre el archivo para escritura
        if (!archivo.is_open()) {
            cout << "[Error] No se pudo crear el archivo.\n"; return;
        }

        // Escribe encabezado en el archivo
        archivo << "====================================\n";
        archivo << "  LISTADO DE MEDICOS - HOSPITAL\n";
        archivo << "====================================\n\n";

        // Escribe cada medico con formato tabular
        for (Medico* m : medicos.obtenerTodos()) {
            archivo << "Nombre       : " << m->getNombre()       << "\n";
            archivo << "Especialidad : " << m->getEspecialidad() << "\n";
            archivo << "Horario      : " << m->getHorario()      << "\n";
            archivo << "Telefono     : " << m->getTelefono()     << "\n";
            archivo << "------------------------------------\n";
        }

        archivo.close(); // Cierra el archivo correctamente
        cout << "[OK] Archivo '" << nombreArchivo << "' generado.\n";
    }

    // -------------------------------------------------------
    // EXPORTAR PACIENTES A ARCHIVO .TXT
    // -------------------------------------------------------
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
    GestorHospitalario gestor; // Crea el gestor del hospital
    gestor.cargarDatosPrecargados(); // Carga medicos y pacientes iniciales

    int opcion; // Variable para la opcion del menu

    // Bucle principal: se repite mientras el usuario no elija 0
    do {
        mostrarMenu();     // Muestra el menu
        cin >> opcion;     // Lee la opcion seleccionada

        // Limpia el buffer de entrada para evitar lecturas erroneas
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            opcion = -1; // Opcion invalida
        }

        // Ejecuta la accion correspondiente a la opcion elegida
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

    } while (opcion != 0); // Continua hasta que el usuario elija salir

    return 0; // Fin del programa (destructores se llaman automaticamente)
}
