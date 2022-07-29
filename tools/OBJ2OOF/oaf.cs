using static OBJ2OOF.OOF;

namespace OBJ2OOF
{
    public static class OAF
    {
        public struct AnimationKeyFrame
        {
            public UInt32 duration;
            public byte interpolation;
            public byte flags;
            public UInt16 points;
            public List<(UInt16 vertex, Vertex v)> data;
        }
        
        public static AnimationKeyFrame? GenerateDiff(ref IndexedModel idx1, ref IndexedModel idx2)
        {
            if (idx1.indices != idx2.indices || idx1.vertices != idx2.vertices)
                return null;
            if (!idx1.indexData.SequenceEqual(idx2.indexData))
            {
                Console.WriteLine("Indices do not match!");
                return null;
            }

            AnimationKeyFrame anim = new AnimationKeyFrame();
            anim.data = new List<(UInt16 vertex, Vertex v)>();
            
            for (UInt16 i = 0; i < idx1.vertices; i++)
            {
                Vertex v1 = idx1.vertexData[i];
                Vertex v2 = idx2.vertexData[i];
                if (!v1.Equals(v2))
                {
                    anim.points++;
                    anim.data.Add((i, v2));
                    Console.WriteLine("Vertex " + i +" changed!");
                    Console.WriteLine(v1.position + " -> " + v2.position);
                    Console.WriteLine(v1.uv + " -> " + v2.uv);
                    Console.WriteLine(v1.normal + " -> " + v2.normal);
                    Console.WriteLine();
                }
            }
            return anim;
        }
        
        public static void SaveAnimation(List<AnimationKeyFrame> frames, string path)
        {
            FileStream fs = File.OpenWrite(path);
            BinaryWriter bw = new BinaryWriter(fs);

            foreach (AnimationKeyFrame f in frames)
            {
                bw.Write(f.duration);
                bw.Write(f.interpolation);
                bw.Write(f.flags);
                bw.Write(f.points);

                foreach((UInt16 n, Vertex v) in f.data)
                {
                    bw.Write(n);
                    
                    bw.Write(v.position.vx); bw.Write(v.position.vy); bw.Write(v.position.vz); bw.Write(v.position.pad);
                    bw.Write(v.uv.vx); bw.Write(v.uv.vy);
                    bw.Write(v.normal.vx); bw.Write(v.normal.vy); bw.Write(v.normal.vz); bw.Write(v.normal.pad);
                }
                
            }
            bw.Close();
        }
    }
}