using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;

using static OBJ2OOF.Types;
using static OBJ2OOF.OBJ;
using static OBJ2OOF.Convert;
using static OBJ2OOF.OOF;
using static OBJ2OOF.OAF;

///TODO: refactor the Main, please.

namespace OBJ2OOF
{
    class Program
    {
        static void Main(string[] args)
        {
            
            if (args.Length == 0)
            {
                PrtHelp();
                return;
            }

            if (args.Length == 1)
            {
                if (!File.Exists(args[0]))
                {
                    PrtHelp();
                    return;
                }
                
                OBJModel obj = LoadOBJ(args[0]);
                IndexedModel idx = ObjToOof(obj);
                //FixModel(ref idx);
                int pos = args[0].LastIndexOf('.');
                SaveIndexedModel(ref idx, args[0].Substring(0, pos).ToUpper()+".OOF");
            }

            if (args.Length > 1)
            {
                string[] flags = args.AsParallel().Where(s => s.StartsWith('-')).ToArray();
                string[] files = args.AsParallel().Where(s => !s.StartsWith('-')).ToArray();

                string dest = flags.FirstOrDefault(s => s.StartsWith("-o"), "./out.OAF").Substring(2);

                if (flags.Contains("-A"))   //Only generate Animation file
                {
                    if (files.Length < 2)
                    {
                        PrtHelp();
                        return;
                    }

                    bool isFirst = true;
                    IndexedModel idx1, idx2 = new IndexedModel();
                    List<AnimationKeyFrame> frames = new List<AnimationKeyFrame>();
                    
                    foreach (string f in files)
                    {
                        if (!File.Exists(f))
                        {
                            Console.WriteLine("File %s does not exist, ignoring!", f);
                            continue;
                        }

                        if (isFirst)
                        {
                            isFirst = false;
                            idx2 = ObjToOof(LoadOBJ(f));
                            continue;
                        }
                        
                        idx1 = idx2;
                        idx2 = ObjToOof(LoadOBJ(f));

                        AnimationKeyFrame? frame = GenerateDiff(ref idx1, ref idx2);
                        if (!frame.HasValue)
                        {
                            PrtHelp();
                            return;
                        }
                        frames.Add(frame.Value);
                    }
                    
                    SaveAnimation(frames, dest);

                    return;
                }
                
                if (flags.Contains("-C"))   //Only convert to OOF
                {
                    foreach (string f in files)
                    {
                        if (!File.Exists(f))
                        {
                            Console.WriteLine("File %s does not exist, ignoring!", f);
                            continue;
                        }
                        OBJModel obj = LoadOBJ(f);
                        IndexedModel idx = ObjToOof(obj);
                        if (flags.Contains("-f"))
                            FixModel(ref idx);
                        int pos = f.LastIndexOf('.');
                        SaveIndexedModel(ref idx, f.Substring(0, pos).ToUpper()+".OOF");
                    }
                }

                if (true)                   //Convert and generate Animation file
                {
                    if (files.Length < 2)
                    {
                        PrtHelp();
                        return;
                    }

                    bool isFirst = true;
                    IndexedModel idx1, idx2 = new IndexedModel();
                    List<AnimationKeyFrame> frames = new List<AnimationKeyFrame>();
                    
                    foreach (string f in files)
                    {
                        if (!File.Exists(f))
                        {
                            Console.WriteLine("File %s does not exist, ignoring!", f);
                            continue;
                        }

                        if (isFirst)
                        {
                            isFirst = false;
                            idx2 = ObjToOof(LoadOBJ(f));
                            if (flags.Contains("-f"))
                                FixModel(ref idx2);
                            SaveIndexedModel(ref idx2, f.Substring(0, f.LastIndexOf('.')).ToUpper()+".OOF");
                            continue;
                        }
                        
                        idx1 = idx2;
                        idx2 = ObjToOof(LoadOBJ(f));
                        if (flags.Contains("-f"))
                            FixModel(ref idx2);
                        SaveIndexedModel(ref idx2, f.Substring(0, f.LastIndexOf('.')).ToUpper()+".OOF");

                        AnimationKeyFrame? frame = GenerateDiff(ref idx1, ref idx2);
                        if (!frame.HasValue)
                        {
                            PrtHelp();
                            return;
                        }
                        frames.Add(frame.Value);
                    }
                    
                    SaveAnimation(frames, dest);

                    return;
                }
            }
        }

        static void PrtHelp()
        {
            Console.WriteLine("OBJ2OOF:");
            Console.WriteLine("\tConverts OBJ files to Obaman Object and Animation Files");
            Console.WriteLine("Usage:");
            Console.WriteLine("\tOBJ2OOF [flags] FILE...");
            Console.WriteLine("\tBy default the program converts the files and generates the animation data");
            Console.WriteLine("Options:");
            Console.WriteLine("\t-f Fix normals' signs");
            Console.WriteLine("\t-A Only generate Animation file");
            Console.WriteLine("\t-C Only Convert to OOF");
        }

    }
}
