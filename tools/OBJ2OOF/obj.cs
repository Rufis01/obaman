using System.Globalization;

using static OBJ2OOF.Types;

namespace OBJ2OOF
{
    public static class OBJ
    {
        public record struct OBJIndex
        {
            public UInt16 vertexIndex;
            public UInt16 uvIndex;
            public UInt16 normalIndex;
            public bool Equals(OBJIndex other)
            {
                return vertexIndex == other.vertexIndex && uvIndex == other.uvIndex && normalIndex == other.normalIndex;
            }
        };

        public struct OBJModel
        {
            public UInt16 vertices;
            public UInt16 uvs;
            public UInt16 normals;
            public UInt16 indices;

            public List<SVECTOR> vertexData;
            public List<DVECTOR> uvData;
            public List<SVECTOR> normalData;
            public List<OBJIndex> indexData;
        };
        
        private const int ONE = 4096;
        private const int SCALE_FACTOR = 4;
        
        public static OBJModel LoadOBJ(string path)
        {
            CultureInfo c = CultureInfo.InvariantCulture;
            StreamReader f = File.OpenText(path);
            OBJModel obj = new OBJModel();
            obj.vertexData = new List<SVECTOR>();
            obj.uvData = new List<DVECTOR>();
            obj.normalData = new List<SVECTOR>();
            obj.indexData = new List<OBJIndex>();
            while (!f.EndOfStream)
            {
                String l = f.ReadLine();
                if (l.StartsWith("v "))
                {
                    String[] sl = l.Split(' ');
                    SVECTOR v = new SVECTOR();
                    v.vx = (Int16)(float.Parse(sl[1], c) * ONE / SCALE_FACTOR);
                    v.vy = (Int16)(float.Parse(sl[2], c) * ONE / SCALE_FACTOR);
                    v.vz = (Int16)(float.Parse(sl[3], c) * ONE / SCALE_FACTOR);
                    obj.vertexData.Add(v);
                    obj.vertices++;
                }
                else if (l.StartsWith("vt"))
                {
                    String[] sl = l.Split(' ');
                    DVECTOR v = new DVECTOR();
                    v.vx = (Int16)(float.Parse(sl[1], c) * ONE);
                    v.vy = (Int16)(float.Parse(sl[2], c) * ONE);
                    obj.uvData.Add(v);
                    obj.uvs++;
                }
                else if (l.StartsWith("vn"))
                {
                    String[] sl = l.Split(' ');
                    SVECTOR v = new SVECTOR();
                    v.vx = (Int16)(float.Parse(sl[1], c) * ONE);
                    v.vy = (Int16)(float.Parse(sl[2], c) * ONE);
                    v.vz = (Int16)(float.Parse(sl[3], c) * ONE);
                    obj.normalData.Add(v);
                    obj.normals++;
                }
                else if (l.StartsWith("f "))
                {
                    String[] sl = l.Split(' ');
                    for (int i = 0; i < 3; i++)
                    {
                        OBJIndex idx = new OBJIndex();
                        String[] ssl = sl[i + 1].Split('/');
                        idx.vertexIndex = (UInt16)(UInt16.Parse(ssl[0]) - 1);
                        idx.uvIndex = (UInt16)(UInt16.Parse(ssl[1]) - 1);
                        idx.normalIndex = (UInt16)(UInt16.Parse(ssl[2]) - 1);
                        obj.indexData.Add(idx);
                        obj.indices++;
                    }
                }
            }
            f.Close();
            return obj;
        }
    }
}

