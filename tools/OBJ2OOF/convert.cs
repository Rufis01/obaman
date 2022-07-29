using System.Collections;

using static OBJ2OOF.OBJ;
using static OBJ2OOF.OOF;

namespace OBJ2OOF
{
    public static class Convert
    {
    
        public static IndexedModel ObjToOof(OBJModel obj)
        {
            IndexedModel idxmod = new IndexedModel();
            idxmod.vertexData = new List<Vertex>();
            idxmod.indexData = new List<UInt16>();
            Hashtable tab = new Hashtable();
            foreach(OBJIndex idx in obj.indexData)
            {
                Vertex vtx = new Vertex();
                vtx.position = obj.vertexData[idx.vertexIndex];
                vtx.normal = obj.normalData[idx.normalIndex];
                vtx.uv = obj.uvData[idx.uvIndex];

                if(tab.ContainsKey(vtx))
                {
                    idxmod.indexData.Add((UInt16)tab[vtx]);
                    idxmod.indices++;
                }
                else
                {
                    tab.Add(vtx, idxmod.vertices);
                    idxmod.vertexData.Add(vtx);
                    idxmod.indexData.Add(idxmod.vertices++);
                    idxmod.indices++;
                }
            }

            return idxmod;
        }
    }
}