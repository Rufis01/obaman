using static OBJ2OOF.Types;

namespace OBJ2OOF
{
    public static class OOF
    {
        public record struct Vertex
        {
            public SVECTOR position;
            public DVECTOR uv;
            public SVECTOR normal;
            public bool Equals(Vertex other)
            {
                return position.Equals(other.position) && uv.Equals(other.uv) && normal.Equals(other.normal);
            }
        };

        public struct IndexedModel
        {
            public UInt16 vertices;
            public UInt16 indices;

            public List<Vertex> vertexData;
            public List<UInt16> indexData;
        }
        
        public static void FixModel(ref IndexedModel idx)
        {
            for (int i = 0; i < idx.indices; i += 3)
            {
                UInt16 idx0 = idx.indexData[i + 0];
                UInt16 idx1 = idx.indexData[i + 1];
                UInt16 idx2 = idx.indexData[i + 2];
                SVECTOR pos0 = idx.vertexData[idx0].position;
                SVECTOR pos1 = idx.vertexData[idx1].position;
                SVECTOR pos2 = idx.vertexData[idx2].position;

                float avgnz = (idx.vertexData[idx0].normal.vz + idx.vertexData[idx1].normal.vz +
                               idx.vertexData[idx2].normal.vz) / 3.0f;

                float iscw = pos0.vx * pos1.vy +
                             pos1.vx * pos2.vy +
                             pos2.vx * pos0.vy -
                             pos0.vx * pos2.vy -
                             pos1.vx * pos0.vy -
                             pos2.vx * pos1.vy;
                if (Math.Sign(avgnz) != Math.Sign(iscw))
                {
                    Console.WriteLine("Fixed face " + i / 3);
                    idx.indexData[i + 1] = idx0;
                    idx.indexData[i + 0] = idx1;
                }
            }
        }
        
        public static void SaveIndexedModel(ref IndexedModel idx, string path)
        {
            FileStream fs = File.OpenWrite(path);
            BinaryWriter bw = new BinaryWriter(fs);

            Console.WriteLine("Vertices: " + idx.vertices);
            Console.WriteLine("Indicies: " + idx.indices);

            bw.Write(idx.vertices);
            bw.Write(idx.indices);

            foreach(Vertex v in idx.vertexData)
            {
                bw.Write(v.position.vx); bw.Write(v.position.vy); bw.Write(v.position.vz); bw.Write(v.position.pad);
                bw.Write(v.uv.vx); bw.Write(v.uv.vy);
                bw.Write(v.normal.vx); bw.Write(v.normal.vy); bw.Write(v.normal.vz); bw.Write(v.normal.pad);
            }

            foreach(ushort i in idx.indexData)
            {
                bw.Write(i);
            }

            bw.Close();
        }
    }
}