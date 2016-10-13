#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>

using std::string;
using std::vector;
using std::pair;

struct TreeNode {
    string str;
    vector<std::shared_ptr<TreeNode>> children;
    TreeNode(const string & s) : str(s) {}

    void print() {
        // std::cout << str;
        if (!children.empty()) {
            std::cout << str << "->";
            for (auto ptr : children) {
                std::cout << " "
                          << ptr->str;
            }
            std::cout << std::endl;
            for (auto ptr : children) {
                ptr->print();
            }
        }
    }
};

using pNode = std::shared_ptr<TreeNode>;

class solution {
  public:
    int lengthLongestPath(const string & input) {
        pNode head = deserialize(input);
        // head->print();
	vector<vector<pNode>> paths = getPaths(head);
	for (auto & path : paths) {
	  for (auto & node : path) {
	    std::cout << node->str << " ";
	  }
	  std::cout << std::endl;
	}

	size_t idx = 0;
	for (size_t i = idx+1; i < paths.size(); ++i)
	  if (paths[i].size() > paths[idx].size())
	    idx = i;

	int len = 0;
	for (size_t i = 0; i < paths[idx].size(); ++i) {
	  len += paths[idx][i]->str.size();
	  if (i != paths[idx].size()-1)
	    std::cout << paths[idx][i]->str << "/";
	  else
	    std::cout << paths[idx][i]->str << std::endl;
	}
	len += paths[idx].size()-1;
	
        
	return len;
    }

  private:
  vector<vector<pNode>> getPaths(const pNode & root) {
    vector<vector<pNode>> paths;
    if (root->children.empty())
      paths.push_back(vector<pNode>{root});
    else {
      for (auto & n : root->children) {
	auto temp = getPaths(n);
	for (auto tmp : temp) {
	  tmp.insert(tmp.begin(), root);
	  paths.push_back(tmp);
	}
      }
    }
    return paths;
  }

  int depth(const pNode & root) {
    int dep;
    if (root == nullptr)
      dep = 0;
    else {
      vector<int> cands;
      if (!root->children.empty()) {
	for (auto & node : root->children)
	  cands.push_back(depth(node));
	dep = *std::max_element(cands.begin(), cands.end());
	dep += 1;
      }
      else
	dep = 1;
    }
    return dep;
  }


    pNode deserialize(const string & serTree) {
        auto vec = split(serTree);
        // std::for_each(vec.begin(), vec.end(), [](const auto & s){std::cout << s.first << " ";});
        // std::cout << std::endl;
        pNode node = nullptr;
        if (!vec.empty()) {
            int tabs = leadingTabs(vec.front().first);
            // std::cout << "tabs = " << tabs << std::endl;
            string curr = vec.front().first;
            curr.erase(curr.begin(), (curr.begin()+tabs));
            node = std::make_shared<TreeNode>(curr);
            vector<size_t> idx;
            for (size_t i = 1; i < vec.size(); ++i)
                if (leadingTabs(vec[i].first) == tabs+1) {
                    idx.push_back(i);
                }
            if (!idx.empty()) {
                for (size_t i = 0; i < idx.size()-1; ++i) {
                    size_t start = vec[idx[i]].second, end = vec[idx[i+1]].second;
                    node->children.push_back(deserialize(serTree.substr(start, end-start)));
                }
                node->children.push_back(deserialize(serTree.substr(vec[idx.back()].second)));
            }
        }
        return node;
    }

    vector<pair<string,size_t>> split(const string & s) {
        vector<pair<string,size_t>> result;
        size_t start = 0, end;
        while (start != string::npos) {
            end = s.find("\n",start);
            result.push_back(std::make_pair(s.substr(start,end-start), start));
            if (end != string::npos)
                start = end+1;
            else
                break;
        }
        
        return result;
    }

    int leadingTabs(const string & str) {
        int count = 0;
        size_t idx = 0;
        while (idx<str.size() && !str.substr(idx,1).compare("\t")) {
            count++;
            idx += 1;
        }
        return count;
    }
};

int main() {
    // string ser{"dir\n\tsubdir1\n\tsubdir2\n\t\tfile.ext"};
    string ser{"dir\n\tsubdir1\n\t\tfile1.ext\n\t\tsubsbudir1\n\tsubdir2\n\t\tsubsubdir2\n\t\t\tfile2.ext"};
    
    solution soln;
    int mlen = soln.lengthLongestPath(ser);
    std::cout << "The max length of the longest path is:\n";
    std::cout << mlen << std::endl;
}
