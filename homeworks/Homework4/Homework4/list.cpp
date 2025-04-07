void listAll(string path, const Class* c)  // two-parameter overload
{
    path += c->name();
    cout << path << endl;
    
    auto sc = c->subclasses(); //const iterator
    for (auto it = sc.begin(); it != sc.end(); it++)
        listAll(path + "=>", *it);
}
