#include<iostream>
#include<vector>
#include<functional>
#include<string>
#include<cstdint>
#include<limits>

#define UINT_MAX std::numeric_limits<unsigned int>::max()
#define UINT8_MAX std::numeric_limits<uint8_t>::max()
#define UINT16_MAX std::numeric_limits<uint16_t>::max()
#define UINT32_MAX std::numeric_limits<uint32_t>::max()
#define UINT64_MAX std::numeric_limits<uint64_t>::max()

class Indexs{
public:
std::vector<int>data;
size_t key;

template<typename T>
size_t hash(const T& value){//通用哈希函数
    return std::hash<T>{}(value);
    }

void get_indexs(size_t key){
    while(key>0){
        this->data.push_back(key & 0b11);
        key>>=2;
        }
    }

template<typename T>
Indexs(const T& value){
        get_indexs(hash(value));
        this->key=hash(value);
    }
void print(){
        std::cout<<"key:"<<this->key<<"\nsize:"<<this->data.size()<<"\ndata:"<<std::endl;
        for(int i=0;i<this->data.size();i++){
            std::cout<<this->data[i]<<" ";}
        std::cout<<std::endl;
    }    
};


class TableNode{
    public:
    unsigned int p0=UINT_MAX;
    unsigned int p1=UINT_MAX;
    unsigned int p2=UINT_MAX;
    unsigned int p3=UINT_MAX;
    unsigned int value=UINT_MAX;
};

class Table{
    public:
    std::vector<TableNode> Nodes;
    std::vector<void*>Values;
    Table() {
        Nodes.push_back(TableNode()); 
    }
    void print(){
        TableNode* TempNode=nullptr;
        std::cout<<"Nodes.size() = "<<Nodes.size()<<std::endl;
        for(int i=0;i<Nodes.size();i++){
            TempNode=&Nodes[i];
            std::cout<<"Node["<<i<<"] p0="<<TempNode->p0<<" p1="<<TempNode->p1<<" p2="<<TempNode->p2<<" p3="<<TempNode->p3<<" value="<<TempNode->value<<std::endl;
        }
        std::cout<<"Values.size() = "<<Values.size()<<std::endl;
        for(int i=0;i<Values.size();i++){
            std::cout<<"Values["<<i<<"]="<<Values[i]<<std::endl;
        }
    }
    void shrink_to_fit(){
        Nodes.shrink_to_fit();
        Values.shrink_to_fit();
    }
    void save(Indexs indexs,void* value){//需要确保void*指向的对象不被销毁
        TableNode* TempNode=&Nodes[0];
        for(int i=0;i<indexs.data.size();i++){
            int index=indexs.data[i];
            unsigned int* index_Node=nullptr;
            switch(index){
                case 0:
                    index_Node=&TempNode->p0;
                    break;
                case 1:
                    index_Node=&TempNode->p1;
                    break;
                case 2: 
                    index_Node=&TempNode->p2;
                    break;
                case 3:
                    index_Node=&TempNode->p3;
                    break;
                default:
                    std::cout<<"[save]索引越界"<<" indexs["<<i<<"]="<<index<<std::endl;
                    break;
            }
            if(*index_Node==UINT_MAX){
                *index_Node=Nodes.size();
                Nodes.push_back(TableNode());
            }
            TempNode=&Nodes[*index_Node];
        }
        TempNode->value=Values.size();
        Values.push_back(value);
    }

    template<typename T>
    T* get_value(Indexs indexs){
        TableNode* TempNode=&Nodes[0];
        for(int i=0;i<indexs.data.size();i++){
            int index=indexs.data[i];
            int* index_Node=nullptr;
            switch(index){
                case 0:
                    index_Node=&TempNode->p0;
                    break;
                case 1:
                    index_Node=&TempNode->p1;
                    break;
                case 2: 
                    index_Node=&TempNode->p2;
                    break;
                case 3:
                    index_Node=&TempNode->p3;
                    break;
                default:
                    std::cout<<"[get]索引越界"<<" indexs["<<i<<"]="<<index<<std::endl;
                    break;
            }
            if(*index_Node==-1){
                *index_Node=Nodes.size();
                Nodes.push_back(TableNode());
            }
            TempNode=&Nodes[*index_Node];
        }
        return static_cast<T*>(Values[TempNode->value]);
    }

    template<typename T>
    size_t hash(const T& value){//通用哈希函数
        return std::hash<T>{}(value);
    }

    Indexs get_indexs(size_t key){
        Indexs indexs(key);
        return indexs;
    }
    
};

int main(){
    Table table=Table();
    Indexs indexs=Indexs(1101);
    indexs.print();
    std::string value="hello";
    table.save(indexs,&value);
    std::cout<<"the value is "<<*(table.get_value<std::string>(indexs))<<std::endl;
    table.shrink_to_fit();
    table.print();
    return 0;

}
