namespace OBJ2OOF
{
    public static class Types
    {

        public record struct SVECTOR
        {
            public Int16 vx, vy, vz, pad;
            public bool Equals(SVECTOR other)
            {
                return vx == other.vx && vy == other.vy && vz == other.vz;
            }

            public override String ToString()
            {
                return vx + ", " + vy + ", " + vz;
            }
        };

        public record struct DVECTOR
        {
            public Int16 vx, vy;
            public bool Equals(DVECTOR other)
            {
                return vx == other.vx && vy == other.vy;
            }

            public override String ToString()
            {
                return vx + ", " + vy + ", ";
            }
        };
    }
}