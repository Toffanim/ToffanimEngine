#pragma once

namespace TE { namespace Core {

    struct stack_allocator {
        stack_allocator( size_t Capacity ) {
            data.data = new char[Capacity];
            data.size = Capacity;
        }

        void* Alloc( size_t AllocSize ) {
            size_t currentTop = marker;
            marker = marker + AllocSize;
            if( marker < 0 ) return nullptr;

            return data.data + currentTop;
        }

        size_t GetMarker() {
            return marker;
        }

        void Free( size_t Marker ) {
            marker = Marker;
        }

        void Clear() {
            marker = 0;
        }

        private:
            array<char> data;
            size_t marker;
    };

    struct pool_allocator {
        pool_allocator(size_t Capacity, size_t PoolObjectSize) {
            data.data = new char [ Capacity * PoolObjectSize ];
            data.size = Capacity * PoolObjectSize;

            Clear();
        }

        void* Alloc() {
            void* resultNode = currentNode;
            currentNode = *(currentNode);
            return resultNode; 
        }

        void* Free( void* Node) {
            *(Node) = currentNode;
            currentNode = Node;
        }

        void Clear() {
            currentNode = data.data;

            for (int i{0}, count{data.size}; i < count; ++i) {
                // Create single_linked list with next pointer directly encoded inside the free memory blocks
                *(data.data) = data.data + (i * PoolObjectSize);
            }
        }
        
        void FreeAll() {
            delete data;
        }

        private:
            array<char> data;
            void* currentNode;
    };

    // toDO(toffa): Do we need a custom "handle" to be able to defragment the heap?
    // I'm not sure as it is not really usefull on Windows, but could it be on other plateforms?
    // (Im looking at you Consoles!)

constexpr size_t MaxEntity = 10000;
static size_t NextID = 1;

struct component {
    bool Activated = false;
};

#define REGISTER_COMPONENT(name) \
    struct name; \
    array<name> name##_array; \
    struct name : public component 
#define ADD_COMPONENT(ID, name) name##_array[ID].Activated = true;
#define REMOVE_COMPONENT(ID, name) name##_array[ID].Activated = false;
    

template < typename T >
struct array {
    T* data = nullptr;
    size_t size = 0; 

    array() = default;
    array(size_t Capacity) {
        data = new T[Capacity];
        size = Capacity;
    }
};

template <typename T>
struct stack {
    void
    push(T Value) {
       data.add(Value);
    }

    T pop() {
        auto result = data[data.size];
        data.size--;
    }

    private:
       vector<T> data; 
};

template <typename T>
struct queue {
    void push(T Value) {
        
    }

    T pop() {

    }

    private:
        vector<T> data;
};

template <typename T>
struct circular_list {
    struct list_node {
        list_node<T>* prev;
        list_node<T>* next;
        T* value;
    };

    void insert() {

    }

    void append(T Value) {
        // TODO(toffa): take an allocator as argument
        // this is aweful to use new like that!
        list_node* newTail = new list_node();
        newTail->prev = root->prev;
        newTail->next = root;
        root->prev = newTail;
    }

    void prepend() {
        // TODO(toffa): take an allocator as argument
        // this is aweful to use new like that!
        list_node* newHead = new list_node();
        newHead->prev = root;
        newHead->next = root->next;
        root->next = newHead;
    }

    private:
        list_node* root;
};

template <typename T>
struct list {

    struct list_node {
        list_node<T>* prev;
        list_node<T>* next;
        T* value;
    };

    void insert() {

    }

    void append(T Value) {
        // TODO(toffa): take an allocator as argument
        // this is aweful to use new like that!
        list_node* newTail = new list_node();
        if(tail) newTail->prev = tail;
        tail = newTail;
    }

    void prepend() {
        // TODO(toffa): take an allocator as argument
        // this is aweful to use new like that!
        list_node* newHead = new list_node();
        if(head) newHead->next = head;
        head = newHead;
    }

    private:
        list_node* head;
        list_node* tail;
};

template <typename T, unsigned int N>
struct small_vector {
    // NOTE(toffa): used for low number of item (<64 bytes or so)
    // it avoids allocating on the heap by allocating on the stack. It also uses a smaller
    // type for capacity and size
    T[N] data;
    unsigned int size = 0;

    void add(T Value) {
        if(++size < N) {
            data[size] = T;
        }
    }
};

template <typename T >
struct vector {
    array<T> data;
    size_t size = 0;

    vector() = default;
    vector(size_t Capacity) {
        data = array<T>(Capacity);
    }
    
    void add(T Value) {
       if (data.data) {
            if( ++size > data.size ) {
                reallocate()
            }

            data.data[size] = Value;
       } 
    }
    // NOTE(toffa): should probably never get called... this is inefficient to remove from the container
    void del(T Value) {
        for(int i{0}, count{size}; i < size; ++i) {
            if ( Value == data.data[i] ) {
                memcpy( data.data + i, data.data + (i+1), (--size)-i);
                break;
            }
        }
    }
    // TODO(MTN5) : highly inefficient when growing from 0 to a moderate number of items
    // maybe go faster when in beginning? anyway should use small_vector for low numbers
    void reallocate() {
        auto NewSize = size*2;
        auto Temp = new T[Newsize];
        memcpy(data.data, Temp, size);
        delete[] data.data;
        data.data = Temp;
        data.size = NewSize;
    }
};

struct ISystemBase {
    virtual void 
    Update( float dt );
};

struct moveSystem:ISystemBase {
    void
    Update( float dt ) override final {
       
    }
};

struct world {
    vector<ISystemBase*> Systems;
    entity_manager Entitites;

    void AddSystem(ISystemBase* S) {
        Systems.add(S);
    }

    void RemoveSystem(ISystemBase* S) {
        Systems.del(S); 
    }

    void RegisterComponent() {
         
    }

    void Update(float dt) {
        for(int i{0}, count{Systems.size}; i < count; ++i) {
            Systems[i].Update(dt);
        }
    }
};

struct entity_manager {
    array<Entity> _Entitites;

    void Init() {
        _Entities.data = new Entity[MaxEntity];
        _Entitites.size = MaxEntity;
   }

    entity& Create() {
        auto& E = _Entitites[NextID]
        E.ID = NextID++;
        E.Activated = true;
        return E;
    }
    void Remove(entity& E) {
        E.ID = 0;
        E.Activated = false;
    };
    void
    AddComponent(entity& E, const char* Comp) {
        ADD_COMPONENT(Comp, E.ID);
    }
    void
    RemoveComponent(entity& E, const char* Comp) {
        REMOVE_COMPONENT(Comp, E.ID);
    }
    array<entity> GetAll();
};

// NOTE(toffa): 0 is reserved for null
struct entity {
    size_t ID = 0;
    flag Activated = false; 
};

REGISTER_COMPONENT(position) {
    vec2f Value;
};
 REGISTER_COMPONENT(gravity) {
     float Value;
 }

struct move_system {
    float dt;
    void Update() {
        auto Entitites = EntityManager.GetAll();
        for(int i; i < Entities.size; ++i) {
            Entities[i].position += Entities[i].velocity * Entities[i].speed * dt; 
        }
    }
};



} // namespace Core
} // namespace TE 