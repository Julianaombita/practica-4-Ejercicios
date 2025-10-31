#include <bits/stdc++.h>
using namespace std;

struct Producto {
    string nombre;
    double precio;
    int stock;
    Producto(string n, double p, int s) : nombre(n), precio(p), stock(s) {}
};

struct ItemCarrito {
    Producto* prod;   // referencia al producto del “catálogo”
    int cantidad;
    ItemCarrito(Producto* p, int c) : prod(p), cantidad(c) {}
    double subtotal() const { return prod->precio * cantidad; }
};

class CarritoCompras {
    vector<ItemCarrito> items;
public:
    // agrega respetando el stock
    bool agregar(Producto& p, int cant) {
        if (cant <= 0 || p.stock < cant) return false;
        // si ya existe, solo incrementa cantidad
        for (auto& it : items) {
            if (it.prod == &p) {
                if (p.stock < cant) return false;
                it.cantidad += cant;
                p.stock -= cant;
                return true;
            }
        }
        items.emplace_back(&p, cant);
        p.stock -= cant;
        return true;
    }

    // elimina unidades (o el ítem completo si queda en 0)
    bool eliminar(Producto& p, int cant) {
        if (cant <= 0) return false;
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].prod == &p) {
                if (cant > items[i].cantidad) return false;
                items[i].cantidad -= cant;
                p.stock += cant;               // se devuelve al stock
                if (items[i].cantidad == 0)    // borra el ítem
                    items.erase(items.begin() + i);
                return true;
            }
        }
        return false; // no estaba en el carrito
    }

    double total() const {
        double t = 0.0;
        for (const auto& it : items) t += it.subtotal();
        return t;
    }

    const vector<ItemCarrito>& getItems() const { return items; }

    bool vacio() const { return items.empty(); }
    void limpiar() { items.clear(); }
};

struct Orden {
    vector<pair<string,int>> detalle; // (nombre,cant)
    double total = 0.0;
};

class Usuario {
    string nombre;
    vector<Orden> historial;
public:
    Usuario(string n) : nombre(n) {}

    // “finalizar compra”: guarda una orden en el historial
    void comprar(const CarritoCompras& c) {
        if (c.vacio()) return;
        Orden o;
        for (const auto& it : c.getItems())
            o.detalle.emplace_back(it.prod->nombre, it.cantidad);
        o.total = c.total();
        historial.push_back(o);
    }

    const vector<Orden>& getHistorial() const { return historial; }

    void imprimirHistorial() const {
        cout << "Historial de " << nombre << ":\n";
        for (size_t i = 0; i < historial.size(); ++i) {
            cout << "  Orden #" << (i+1) << "  Total: $" << fixed << setprecision(2)
                 << historial[i].total << "\n";
            for (auto& par : historial[i].detalle)
                cout << "    - " << par.first << " x" << par.second << "\n";
        }
    }
};

int main() {
    // catálogo
    Producto p1("Laptop", 3500.0, 5);
    Producto p2("Mouse", 50.0, 20);
    Producto p3("Headset", 120.0, 10);

    CarritoCompras carrito;
    Usuario u("Steven");

    carrito.agregar(p1, 1);   // OK
    carrito.agregar(p2, 2);   // OK
    carrito.eliminar(p2, 1);  // devuelve 1 mouse al stock
    carrito.agregar(p3, 3);   // OK

    cout << "Total actual: $" << fixed << setprecision(2) << carrito.total() << "\n";

    // cerrar compra → guarda en historial
    u.comprar(carrito);

    u.imprimirHistorial();
    return 0;
}
