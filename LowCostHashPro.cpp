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

class TableNode_UINT8{
    public:
    uint8_t p0=UINT8_MAX;
    uint8_t p1=UINT8_MAX;
    uint8_t p2=UINT8_MAX;
    uint8_t p3=UINT8_MAX;
    uint8_t value=UINT8_MAX;
};
class TableNode_UINT16{
    public:
    uint16_t p0=UINT16_MAX;
    uint16_t p1=UINT16_MAX;
    uint16_t p2=UINT16_MAX;
    uint16_t p3=UINT16_MAX;
    uint16_t value=UINT16_MAX;
};
class TableNode_UINT32{
    public:
    uint32_t p0=UINT32_MAX;
    uint32_t p1=UINT32_MAX;
    uint32_t p2=UINT32_MAX;
    uint32_t p3=UINT32_MAX;
    uint32_t value=UINT32_MAX;
};
class TableNode_UINT64{
    public:
    uint64_t p0=UINT64_MAX;
    uint64_t p1=UINT64_MAX;
    uint64_t p2=UINT64_MAX;
    uint64_t p3=UINT64_MAX;
    uint64_t value=UINT64_MAX;
};


class Table{
    public:
    void* Nodes=nullptr;
    uint8_t typeId=8;//8 16 32 64
    void* MAX=nullptr;
    std::vector<void*>Values;
    Table() {
        std::vector<TableNode_UINT8>* vec = new std::vector<TableNode_UINT8>;
        // Nodes.push_back(TableNode()); 
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

    void Nodes_UP(uint8_t typeId_goal){
        switch (this->typeId)
        {
        case 8:{
            std::vector<TableNode_UINT8>* vec=reinterpret_cast<std::vector<TableNode_UINT8>*>(this->Nodes);
            switch(typeId_goal){
                case 16:{
                    std::vector<TableNode_UINT16>* newvec=new std::vector<TableNode_UINT16>;
                    for(int i=0;i<vec->size();i++){
                        TableNode_UINT16 newNode=TableNode_UINT16();
                        newNode.p0=vec->at(i).p0;
                        newNode.p1=vec->at(i).p1;
                        newNode.p2=vec->at(i).p2;
                        newNode.p3=vec->at(i).p3;
                        newNode.value=vec->at(i).value;
                        newvec->push_back(newNode);
                    }
                    delete vec;//释放指针会同步调用析构
                    this->Nodes=newvec;
                    this->typeId=16;
                    break;
                }
                case 32:{
                    std::vector<TableNode_UINT32>* newvec=new std::vector<TableNode_UINT32>;
                    for(int i=0;i<vec->size();i++){
                        TableNode_UINT32 newNode=TableNode_UINT32();
                        newNode.p0=vec->at(i).p0;
                        newNode.p1=vec->at(i).p1;
                        newNode.p2=vec->at(i).p2;
                        newNode.p3=vec->at(i).p3;
                        newNode.value=vec->at(i).value;
                        newvec->push_back(newNode);
                    }
                    delete vec;//释放指针会同步调用析构
                    this->Nodes=newvec;
                    this->typeId=32;
                    break;
                }
                case 64:{
                    std::vector<TableNode_UINT64>* newvec=new std::vector<TableNode_UINT64>;
                    for(int i=0;i<vec->size();i++){
                        TableNode_UINT64 newNode=TableNode_UINT64();
                        newNode.p0=vec->at(i).p0;
                        newNode.p1=vec->at(i).p1;
                        newNode.p2=vec->at(i).p2;
                        newNode.p3=vec->at(i).p3;
                        newNode.value=vec->at(i).value;
                        newvec->push_back(newNode);
                    }
                    delete vec;//释放指针会同步调用析构
                    this->Nodes=newvec;
                    this->typeId=64;
                    break;
                }
                default:{
                    std::cout<<"Error: typeId_goal is not supported"<<std::endl;
                }
            }
        break;
        }
        case 16:{
            std::vector<TableNode_UINT16>* vec=reinterpret_cast<std::vector<TableNode_UINT16>*>(this->Nodes);
            switch(typeId_goal){
                case 32:{
                    std::vector<TableNode_UINT32>* newvec=new std::vector<TableNode_UINT32>;
                    for(int i=0;i<vec->size();i++){
                        TableNode_UINT32 newNode=TableNode_UINT32();
                        newNode.p0=vec->at(i).p0;
                        newNode.p1=vec->at(i).p1;
                        newNode.p2=vec->at(i).p2;
                        newNode.p3=vec->at(i).p3;
                        newNode.value=vec->at(i).value;
                        newvec->push_back(newNode);
                    }
                    delete vec;//释放指针会同步调用析构
                    this->Nodes=newvec;
                    this->typeId=16;
                    break;
                }
                case 64:{
                    std::vector<TableNode_UINT64>* newvec=new std::vector<TableNode_UINT64>;
                    for(int i=0;i<vec->size();i++){
                        TableNode_UINT64 newNode=TableNode_UINT64();
                        newNode.p0=vec->at(i).p0;
                        newNode.p1=vec->at(i).p1;
                        newNode.p2=vec->at(i).p2;
                        newNode.p3=vec->at(i).p3;
                        newNode.value=vec->at(i).value;
                        newvec->push_back(newNode);
                    }
                    delete vec;//释放指针会同步调用析构
                    this->Nodes=newvec;
                    this->typeId=16;
                    break;
                }
                default:{
                    std::cout<<"Error: typeId_goal is not supported"<<std::endl;
                }
            }
        break;
        }
        case 32:{
            std::vector<TableNode_UINT32>* vec=reinterpret_cast<std::vector<TableNode_UINT32>*>(this->Nodes);
            switch(typeId_goal){
                case 64:{
                    std::vector<TableNode_UINT64>* newvec=new std::vector<TableNode_UINT64>;
                    for(int i=0;i<vec->size();i++){
                        TableNode_UINT64 newNode=TableNode_UINT64();
                        newNode.p0=vec->at(i).p0;
                        newNode.p1=vec->at(i).p1;
                        newNode.p2=vec->at(i).p2;
                        newNode.p3=vec->at(i).p3;
                        newNode.value=vec->at(i).value;
                        newvec->push_back(newNode);
                    }
                    delete vec;//释放指针会同步调用析构
                    this->Nodes=newvec;
                    this->typeId=16;
                    break;
                }
                default:
                    std::cout<<"Error: typeId_goal is not supported"<<std::endl;
            }
        break;
        }
        default:
            std::cout<<"Error: this->typeId is not supported"<<std::endl;
        }
    }

    bool check_range(int size){
        switch (this->typeId)
        {
        case 8:
            std::vector<TableNode_UINT8>* vec=reinterpret_cast<std::vector<TableNode_UINT8>*>(Nodes);
            if(vec->size()-1+size>=UINT8_MAX&&vec->size()-1+size<UINT16_MAX){
                this->Nodes_UP(16);
                return true;
            }
            else if(vec->size()-1+size>=UINT8_MAX&&vec->size()-1+size<UINT32_MAX){
                this->Nodes_UP(32);
                return true;
            }
            else if(vec->size()-1+size>=UINT8_MAX&&vec->size()-1+size<UINT64_MAX){
                this->Nodes_UP(64);
                return true;
            }
            return false;
            break;
        case 16:
            std::vector<TableNode_UINT16>* vec=reinterpret_cast<std::vector<TableNode_UINT16>*>(Nodes);
            if(vec->size()-1+size>=UINT16_MAX&&vec->size()-1+size<UINT32_MAX){
                this->Nodes_UP(32);
                return true;
            }
            else if(vec->size()-1+size>=UINT16_MAX&&vec->size()-1+size<UINT64_MAX){
                this->Nodes_UP(64);
                return true;
            }
            return false;
            break;
        case 32:
            std::vector<TableNode_UINT32>* vec=reinterpret_cast<std::vector<TableNode_UINT32>*>(Nodes);
            if(vec->size()-1+size>=UINT32_MAX&&vec->size()-1+size<UINT64_MAX){
                this->Nodes_UP(64);
                return true;
            }
            return false;
            break;
        default:
            std::cout<<"Error: typeId:"<<this->typeId<<std::endl;
            break;
        }
    }

    void save(Indexs indexs,void* value){//需要确保void*指向的对象不被销毁
        if(check_range(indexs.data.size())){
            this->save(indexs,value);//vector扩展，重新刷新
        }
        switch (this->typeId)
        {
        case 8:
            std::vector<TableNode_UINT8>* vec=reinterpret_cast<std::vector<TableNode_UINT8>*>(Nodes);
            TableNode_UINT8* TempNode=&(*vec)[0];
        for(int i=0;i<indexs.data.size();i++){
            int index=indexs.data[i];
            uint8_t* index_Node=nullptr;
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
            if(*index_Node==UINT8_MAX){
                *index_Node=vec->size();
                vec->push_back(TableNode_UINT8());
            }
            TempNode=&(*vec)[*index_Node];
        }
        TempNode->value=Values.size();
        Values.push_back(value);
            break;
        
        default:
            break;
        }
        
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
