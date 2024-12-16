#ifndef BEAVER_AI_H
#define BEAVER_AI_H

#include <components/state.hpp>

#include <vector>
#include <variant>
#include <memory>

namespace beaver
{
namespace BT
{
	enum class NODE_RESULT{SUCCESS, FAILURE, RUNNING};

	struct __btnode
	{
		std::vector<std::unique_ptr<__btnode>> _childs;
		std::size_t _current_child {0};
		virtual ~__btnode() = 0;
		virtual NODE_RESULT update() = 0;

		void reset()
		{
			_current_child = 0;
			for (auto& child: _childs)
				child->reset();
		};
	};

	struct selector : __btnode
	{
		virtual NODE_RESULT update() 
		{
			NODE_RESULT child_rs {_childs.at(_current_child)->update()};
			if (child_rs == NODE_RESULT::FAILURE)
			{
				if (_current_child == _childs.size() - 1)
				{
					reset();
					return NODE_RESULT::FAILURE;
				}
				else 
				{
					_current_child++;
					return NODE_RESULT::RUNNING;
				};
			}
			else
			{
				reset();
				return child_rs;
			};
		};
	};

	struct sequence : __btnode
	{
		virtual NODE_RESULT update()
		{
			NODE_RESULT child_rs {_childs.at(_current_child)->update()};
			if (child_rs == NODE_RESULT::SUCCESS)
			{
				if (_current_child == _childs.size() - 1) 
				{
					reset();
					return NODE_RESULT::SUCCESS;
				}
				else
				{
					_current_child++;
					return NODE_RESULT::RUNNING;
				};
			}
			else
			{
				reset();
				return child_rs;
			};
		};
	};

	//struct parallel
	//{
	//};

	struct tree
	{
		std::unique_ptr<__btnode> _root;

		NODE_RESULT update()
		{
			return _root->update();
		};

		void reset()
		{
			_root->reset();
		};
	};

};


//TODO
namespace GOAP
{
	
};

};



#endif
