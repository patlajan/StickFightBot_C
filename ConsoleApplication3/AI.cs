using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication3
{
    public struct Reward
    {
        public double Value;
        public int TimesExplored;

        public Reward(double rewardValue, int timesExplored)
        {
            this.Value = rewardValue;
            this.TimesExplored = timesExplored;
        }
    }

    class AI
    {
        Dictionary<string, Dictionary<string, Reward>> stateMap;
        Random random;
        public delegate void TakeAction(string action);
        TakeAction actionMethod;

        public AI(TakeAction actionMethod)
        {
            random = new Random();
            this.actionMethod = actionMethod;
        }

        IList<string> GetPossibleActions()
        {
            return new string[3] { "hitRight", "hitLeft", "wait" };
        }

        string GetRandomAction()
        {
            return GetPossibleActions()[random.Next(0, 3)];
        }

        public void Act(string state) {
            var action = this.GetAction(state);
            this.actionMethod(action);
            var reward = 1; //TODO: calculate actual reward;
            this.Recalc(state, action, reward);
        }

        public void Recalc(string state, string action, double reward)
        {
            if (!this.stateMap.ContainsKey(state))
            {
                //create one
                var map2 = new Dictionary<string, Reward>();
                map2.Add(action, new Reward(reward, 1));
                this.stateMap.Add(state, map2);
            }
            else
            {
                var map2 = this.stateMap[state];
                if (!map2.ContainsKey(action))
                {
                    map2.Add(action, new Reward(reward, 1));
                }
                else
                {
                    var oldReward = map2[action].Value;
                    var timesExplored = map2[action].TimesExplored;
                    var newReward = (oldReward * timesExplored / 2 + reward) / (timesExplored / 2 + 1);
                    map2[action] = new Reward(newReward, timesExplored + 1);
                }
            }
        }

        public string GetAction(string state)
        {
            if (!this.stateMap.ContainsKey(state))
            {
                //nothing found, select action on random
                return GetRandomAction();
            }
            else
            {
                var map2 = this.stateMap[state];
                foreach (var item in map2)
                {
                    //TODO: Parametrize or tweak reward value threshold
                    if (item.Value.Value > 0.5)
                    {
                        //reward is good but we decide to explore anyway;
                        if (random.Next(0, item.Value.TimesExplored) == 1)
                            break;

                        //reward is good, so we use that.
                        return item.Key;
                    }
                }

                //if for cycle didn't return a value, this means no viable options are available in memory, or it decided to explore;
                //select action on random
                return GetRandomAction();
            }
        }
    }
}
