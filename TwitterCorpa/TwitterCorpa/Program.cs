using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Tweetinvi;

namespace TwitterCorpa
{
    class Program
    {
        #region data structs
        public struct LangData : IComparable, IComparable<string>
        {
            public string langName { get; }
            public int occurences { get; set; }
            public LangData(string n)
            {
                this.langName = n;
                this.occurences = 0;
            }
            public void increment()
            {
                occurences++;
            }
            public int CompareTo(string other)
            {
                return langName.CompareTo(other);
            }
            public int CompareTo(object obj)
            {
                if (obj is string)
                    return langName.CompareTo(obj);
                return langName.CompareTo(((LangData)obj).langName);
            }
        }
        #endregion
        #region program vars
        public const string PATH = @"C:\Users\Felis\Desktop\programming\SuperComputing\WordData\data\";
        public const string VERSION = "1003";
        public const string PREFIX = @"twitter-";
        public const Tweetinvi.Models.Language TargetLanguage = Tweetinvi.Models.Language.English;
        public const bool ExcludeReplies = false;
        public const bool ExcludeRts = false;
        public static readonly int? tweetSampleSize = null; // 10000;
        public static readonly long? sampleTime = 1000 * 60 * 60 * 24 * 3; // Milliseconds 3 days
        public const int lengthSampleSize = 10000;
        public const string padding = "\t\t\t\t\t\t";
        #endregion
        #region Utils
        public static double roundToPrint(double d)
        {
            return (Math.Round(d * 10) / 10);
        }
        public static void IOInit(string path)
        {
            if (!File.Exists(path))
            {
                using (StreamWriter w = File.AppendText(path)) ;
            }
            else
            {
                using (StreamWriter sw = new StreamWriter(path, false))
                {
                    sw.Write("");
                }
            }
        }
        public static void setupIO()
        {
            Console.WriteLine("-");
            Console.WriteLine("-");
            Console.SetCursorPosition(0, Console.CursorTop - 1);
        }
        #endregion
        public static void somethingWentHorriblyWrong()
        {
            Console.ReadLine();
            Environment.Exit(0);
        }
        static void Main(string[] _args)
        {
            Console.WindowWidth = Math.Min(150, Console.WindowWidth * 2);
            Console.WriteLine("TwitterCorpa Generation process initialized.");
            setupIO();
            // Credentials
            Auth.SetUserCredentials(
                "BJbBT6V5xZPr42Krvg44Tx1mj",
                "JdQ6ujlP2VkExZShwt7R9iqpKMeox0RNOm07tItVVuSzGQ1fIM",
                "2618817260-Fsnkv5Yayr0UpMwrIvMaBxCcRuvFRYoYmW64Tmb",
                "BsMtbWzZNE1UxcEotfQwnrwkHnha7aQQiYp46N1tHGakw");

            // Publish the Tweet "Hello World" on your Timeline
            //Tweet.PublishTweet("Hello World!\nSent from my code");

            // IO setup
            IOInit(PATH + PREFIX + VERSION + "-" + "stats");
            IOInit(PATH + PREFIX + VERSION + "-" + "rawdata");
            IOInit(PATH + PREFIX + VERSION + "-" + "langstats");
            IOInit(PATH + PREFIX + VERSION + "-" + "lengthstats");
            IOInit(PATH + PREFIX + VERSION + "-" + "debuglog");

            // Stats
            int totalTweets = 0;
            int usableTweets = 0;
            int numRts = 0;
            int numReplies = 0;
            List<int> lengths = new List<int>();

            // Languages
            Array _langs = Enum.GetValues(typeof(Tweetinvi.Models.Language));
            LangData[] langs = new LangData[_langs.Length];
            for (int i = 0; i < _langs.Length; i++)
            {
                //Console.WriteLine(Enum.GetName(typeof(Tweetinvi.Models.Language), i) + "\t" + _langs.GetValue(i).ToString());
                langs[i] = new LangData(_langs.GetValue(i).ToString());
            }
            Array.Sort<LangData>(langs);
            _langs = null;

            // Mechanics
            int IODivisions = 1000;
            string[] tweets = new string[IODivisions];
            int currentIODivision = 0;
            long startTime = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
            // This code will stop working 292.5 billion years post unix epoch
            long elapsedTime;

            // Initiate String
            var stream = Tweetinvi.Stream.CreateSampleStream(); // Have to call Tweetinvi.Stream because Stream interferes with System.IO
            stream.StallWarnings = true;
            stream.TweetReceived += (sender, args) =>
            {
                Console.SetCursorPosition(0, Console.CursorTop - 1);
                Console.Write("Recieved\t" + totalTweets.ToString() + "\tWith\t" + usableTweets.ToString() + "\tusable tweets");
                Console.SetCursorPosition(0, Console.CursorTop + 1);
                elapsedTime = (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond) - startTime;
                double tweetsPerMs = (double)totalTweets / (double)elapsedTime;
                double uTweetsPerMs = (double)usableTweets / (double)elapsedTime;
                if(tweetSampleSize != null)
                    Console.Write(roundToPrint(tweetsPerMs * 1000) + " total tweets/sec\t" + roundToPrint(uTweetsPerMs * 1000) + " Usable tweets/sec\t" + roundToPrint(((((double)tweetSampleSize / uTweetsPerMs) - elapsedTime) / 1000) / 60) + "\t minutes left");
                else if (sampleTime != null)
                    Console.Write(roundToPrint(tweetsPerMs * 1000) + " total tweets/sec\t" + roundToPrint(uTweetsPerMs * 1000) + " Usable tweets/sec\t" + roundToPrint((((double)sampleTime - elapsedTime) / 1000) / 60) + "\t minutes left\t" + roundToPrint(((((long)sampleTime - elapsedTime) / 1000) / 60) / 60) + "\thours left");
                if (tweetSampleSize != null ? (usableTweets >= tweetSampleSize) : (elapsedTime > sampleTime))//totalTweets
                {
                    // Write totals
                    using (StreamWriter sw = new StreamWriter(PATH + PREFIX + VERSION + "-" + "stats", true))
                    {
                        sw.WriteLine("Total tweets\t" + totalTweets);
                        sw.WriteLine("Total usable tweets\t" + usableTweets);
                        sw.WriteLine("Number of retweets\t" + numRts);
                        sw.WriteLine("Number of replies\t" + numReplies);
                        sw.WriteLine("Tweets/sec\t" + (tweetsPerMs * 1000));
                        sw.WriteLine("Usable tweets/sec\t" + roundToPrint(uTweetsPerMs * 1000));
                        sw.WriteLine("Elapsed milliseconds\t" + elapsedTime);
                    }
                    using (StreamWriter sw = new StreamWriter(PATH + PREFIX + VERSION + "-" + "langstats", true))
                    {
                        for (int i = 0; i < langs.Length; i++)
                        {
                            sw.WriteLine((langs[i].langName == null ? "Null" : langs[i].langName) + "\t" + langs[i].occurences);
                        }
                    }
                    using (StreamWriter sw = new StreamWriter(PATH + PREFIX + VERSION + "-" + "lengthstats", true))
                    {
                        for (int i = 0; i < lengths.Count; i++)
                        {
                            sw.WriteLine(lengths[i]);
                        }
                    }
                    using (StreamWriter sw = new StreamWriter(PATH + PREFIX + VERSION + "-" + "rawdata", true))
                    {
                        for (int i = 0; i < currentIODivision; i++) // only up to currentIODivision - 1 has been written
                        {
                            sw.WriteLine(tweets[i].Replace('\n', '\uE032'));
                        }
                    }
                    stream.StopStream();
                    return;
                }
                // 
                totalTweets++;

                // Evaluate language stats before anything else
                var language = args.Tweet.Language;
                string lstr = language.ToString();
                int index = Array.BinarySearch(langs, lstr);
                langs[index].increment();

                // Length stats
                if(lengths.Count < lengthSampleSize)
                    lengths.Add(args.Tweet.PublishedTweetLength);

                // Do stats, then return
                // Not RT
                if (args.Tweet.IsRetweet)
                {
                    numRts++;
                }
                // Not Reply
                if (args.Tweet.InReplyToStatusId != null)
                {
                    numReplies++;
                }
                // Not RT
                if (ExcludeRts && args.Tweet.IsRetweet)
                {
                    return;
                }
                // Not Reply
                if (ExcludeReplies && args.Tweet.InReplyToStatusId != null)
                {
                    return;
                }

                // Now exit if in other lang
                if (language != TargetLanguage)
                {
                    return;
                }

                // Tweet handling
                usableTweets++;
                if(currentIODivision < IODivisions)
                {
                    tweets[currentIODivision++] = args.Tweet.FullText;
                } else
                {
                    using (StreamWriter sw = new StreamWriter(PATH + PREFIX + VERSION + "-" + "rawdata", true))
                    {
                        for(int i = 0; i < tweets.Length; i++)
                        {
                            sw.WriteLine(tweets[i].Replace('\n', '\uE032'));
                        }
                    }
                    currentIODivision = 0;
                    tweets[currentIODivision++] = args.Tweet.FullText;
                }
            };
            stream.WarningFallingBehindDetected += (sender, args) =>
            {
                Console.SetCursorPosition(0, Console.CursorTop - 1);
                Console.WriteLine("Falling behind!" + padding);
                long signature = (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond);
                Console.WriteLine("Warning: " + args.WarningMessage + padding);
                Console.WriteLine("Debug info in " + PATH + PREFIX + VERSION + "-" + "debuglog" + " with signature " + signature);
                using (StreamWriter sw = new StreamWriter(PATH + PREFIX + VERSION + "-" + "debuglog", true))
                {
                    sw.WriteLine("Sig-" + signature);
                    sw.WriteLine(args.WarningMessage);
                }
                setupIO();
                //somethingWentHorriblyWrong();
            };
            stream.LimitReached += (sender, args) =>
            {
                Console.SetCursorPosition(0, Console.CursorTop - 1);
                Console.WriteLine("Limit reached!" + padding);
                long signature = (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond);
                Console.WriteLine("Tweets missed: " + args.NumberOfTweetsNotReceived + padding);
                Console.WriteLine("Debug info in " + PATH + PREFIX + VERSION + "-" + "debuglog" + " with signature " + signature);
                using (StreamWriter sw = new StreamWriter(PATH + PREFIX + VERSION + "-" + "debuglog", true))
                {
                    sw.WriteLine("Sig-" + signature);
                    sw.WriteLine(args.NumberOfTweetsNotReceived);
                }
                setupIO();
                //somethingWentHorriblyWrong();
            };
            stream.DisconnectMessageReceived += (sender, args) =>
            {
                Console.SetCursorPosition(0, Console.CursorTop - 1);
                Console.WriteLine("Disconnect message recieved!" + padding);
                long signature = (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond);
                Console.WriteLine("Debug info in " + PATH + PREFIX + VERSION + "-" + "debuglog" + " with signature " + signature);
                using (StreamWriter sw = new StreamWriter(PATH + PREFIX + VERSION + "-" + "debuglog", true))
                {
                    sw.WriteLine("Sig-" + signature);
                    sw.WriteLine(args.DisconnectMessage);
                }
                setupIO();
                somethingWentHorriblyWrong();
            };
            stream.StreamStopped += (sender, args) =>
            {
                Console.SetCursorPosition(0, Console.CursorTop - 1);
                if (args.DisconnectMessage == null && args.Exception == null)
                    return; // probably manual stream.stop();
                Console.WriteLine("Stream Stopped!" + padding);
                long signature = (DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond);
                Console.WriteLine("Debug info in " + PATH + PREFIX + VERSION + "-" + "debuglog" + " with signature " + signature);
                using(StreamWriter sw = new StreamWriter(PATH + PREFIX + VERSION + "-" + "debuglog", true))
                {
                    sw.WriteLine("Sig-" + signature);
                    sw.WriteLine(args.Exception);
                    sw.WriteLine(args.DisconnectMessage);
                }
                Console.WriteLine("Attempting to restart stream...");
                
                stream.StartStream();
                //somethingWentHorriblyWrong();
                setupIO();
            };
            stream.StartStream();
        }
    }
}
