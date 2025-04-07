//#include <iostream>
//#include <string>
//using namespace std;
//
//======================================================
//Your declarations and implementations would go here
//======================================================

class Landmark{
    public:
        Landmark(string name) : _name(name){}
        virtual ~Landmark(){}
        string name() const{
            return _name;
        };
        virtual string color() const{
            return "yellow";
        };
        virtual string icon() const = 0;
    private:
        string _name;
};

class Hotel : public Landmark{
    public:
        Hotel(string name) : Landmark(name){}
        ~Hotel(){
            cout << "Destroying the hotel " << name() << "." << endl;
        }
        string icon() const{
            return "bed";
        }
};

//capacity threshold, < for small icon and >= for large icon
int RESTAURANT_CAPACITY_THRESHOLD = 40;

class Restaurant : public Landmark{
    public:
        Restaurant(string name, int capacity) : Landmark(name) , _capacity(capacity){}
        ~Restaurant(){
            cout << "Destroying the restaurant " << name() << "." << endl;
        }
        string icon() const{
            if (_capacity < RESTAURANT_CAPACITY_THRESHOLD) return "small knife/fork";
            return "large knife/fork";
        }
    private:
        int _capacity;
};

class Hospital : public Landmark{
    public:
        Hospital(string name) : Landmark(name){}
        ~Hospital(){
            cout << "Destroying the hospital " << name() << "." << endl;
        }
        string color() const{
            return "blue";
        }
        string icon() const{
            return "H";
        }
};

//======================================================
//======================================================
//
//void display(const Landmark* lm)
//{
//    cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
//     << lm->name() << "." << endl;
//}
//
//int main()
//{
//    Landmark* landmarks[4];
//    landmarks[0] = new Hotel("Westwood Rest Good");
//      // Restaurants have a name and seating capacity.  Restaurants with a
//      // capacity under 40 have a small knife/fork icon; those with a capacity
//      // 40 or over have a large knife/fork icon.
//    landmarks[1] = new Restaurant("Bruin Bite", 30);
//    landmarks[2] = new Restaurant("La Morsure de l'Ours", 100);
//    landmarks[3] = new Hospital("UCLA Medical Center");
//
//    cout << "Here are the landmarks." << endl;
//    for (int k = 0; k < 4; k++)
//    display(landmarks[k]);
//
//      // Clean up the landmarks before exiting
//    cout << "Cleaning up." << endl;
//    for (int k = 0; k < 4; k++)
//    delete landmarks[k];
//}
