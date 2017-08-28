// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* ======================================================================== 
    $File: TEPhysics\bvh.h $
    $Created: 01-01-2017 00h00m00s $
    $Modified: 22-08-2017 01h20m39s $
    $Revision: $
    $Creator : TOFFANIN Marc $
    $Notice: Licensed under GNU GPL $
   ======================================================================== */
#ifndef BVH_h
#define BVH_h

#include "TECore/actor.h"
#include "TECore/math.h"
#include <vector>
#include <memory>
#include <iterator>
#include <algorithm>

namespace TE
{
	namespace Physic
	{
		typedef std::vector< std::shared_ptr<Core::actor> > bvh_object_list;

		struct ray
		{
			glm::vec3 Origin;
			glm::vec3 Direction;
			int ObjectID;
			glm::vec3 InverseDirection;
			double T;  //IMPORTANT(Marc) : Make this Inf
		};

		struct bvh_node
		{
			enum type { LEAF, ROOT, INTERNAL };
			type Type;
			std::shared_ptr<bvh_node> Left;
			std::shared_ptr<bvh_node> Right;
			std::shared_ptr<bvh_node> Parent;
			Math::bounding_box BoundingBox;
			int ObjectID;

			bvh_node(type Type, Math::bounding_box BoundingBox) : Type(Type), BoundingBox(BoundingBox){};
		};

		class bvh
		{
		public:
			bvh(bvh_object_list& Objects)
			{
				Build(Objects, nullptr, 0, (const unsigned int)Objects.size());
			}


			bool GetIntersection(ray& Ray) const
			{
				double t;
				if (!Intersection(Tree[0]->BoundingBox, Ray, t))
					return false;

				// NOTE(MArc) : This is unpack, because if we do
				// return (ProcessNode(*Tree[0]->Left, Ray) || ProcessNode(*Tree[0]->Right, Ray));
				// and the first exp is True, then the second exp is not evaluated
                bool ResultLeft=false, ResultRight = false;
				if (Tree[0]->Left)
					ResultLeft = ProcessNode(*Tree[0]->Left, Ray);
				if (Tree[0]->Right)
					ResultRight = ProcessNode(*Tree[0]->Right, Ray);
				return (ResultLeft || ResultRight);
			}

		private:
			std::unique_ptr<bvh_object_list> Objects;
			std::vector< std::shared_ptr<bvh_node> > Tree;


			Math::bounding_box ComputeNodeAABB(bvh_object_list& Objects, int Start, int End) const
			{
				Math::bounding_box NodeAABB = {};
				Math::bounding_box tmp;
				for (auto it = Objects.begin(); it != Objects.end(); ++it)
				{
					tmp = (*it)->GetAABB();
					NodeAABB.UpperBound.x = std::max(NodeAABB.UpperBound.x, tmp.UpperBound.x);
					NodeAABB.LowerBound.x = std::min(NodeAABB.LowerBound.x, tmp.LowerBound.x);
					NodeAABB.UpperBound.y = std::max(NodeAABB.UpperBound.y, tmp.UpperBound.y);
					NodeAABB.LowerBound.y = std::min(NodeAABB.LowerBound.y, tmp.LowerBound.y);
					NodeAABB.UpperBound.z = std::max(NodeAABB.UpperBound.z, tmp.UpperBound.z);
					NodeAABB.LowerBound.z = std::min(NodeAABB.LowerBound.z, tmp.LowerBound.z);
				}
				return(NodeAABB);
			}

			bool ProcessNode(const bvh_node& Node, ray& Ray) const
			{
				double t;
				if (!Intersection(Node.BoundingBox, Ray, t))
				{
					return false;
				}
				else
				{
					if (Node.Type != bvh_node::LEAF)
					{
						// NOTE(MArc) : This is unpack, because if we do
						// return (ProcessNode(*Tree[0]->Left, Ray) || ProcessNode(*Tree[0]->Right, Ray));
						// and the first exp is True, then the second exp is not evaluated
						bool ResultLeft=false, ResultRight=false;
						if (Node.Left)
							ResultLeft = ProcessNode(*Node.Left.get(), Ray);
						if (Node.Right)
							ResultRight = ProcessNode(*Node.Right.get(), Ray);
						return (ResultLeft || ResultRight);
					}
					else
					{
						if (t < Ray.T)
						{
							Ray.T = t;
							Ray.ObjectID = Node.ObjectID;
						}
						return true;
					}
				}
			}

			bool Intersection(const Math::bounding_box& b, const ray& r, double& t) const
			{
				double tx1 = (b.LowerBound.x - r.Origin.x)*r.InverseDirection.x;
				double tx2 = (b.UpperBound.x - r.Origin.x)*r.InverseDirection.x;

				double tmin = std::min(tx1, tx2);
				double tmax = std::max(tx1, tx2);

				double ty1 = (b.LowerBound.y - r.Origin.y)*r.InverseDirection.y;
				double ty2 = (b.UpperBound.y - r.Origin.y)*r.InverseDirection.y;

				tmin = std::max(tmin, std::min(ty1, ty2));
				tmax = std::min(tmax, std::max(ty1, ty2));

				double tz1 = (b.LowerBound.z - r.Origin.z)*r.InverseDirection.z;
				double tz2 = (b.UpperBound.z - r.Origin.z)*r.InverseDirection.z;

				tmin = std::max(tmin, std::min(tz1, tz2));
				tmax = std::min(tmax, std::max(tz1, tz2));

				t = tmin;
				return tmax >= std::max(0.0, tmin);
			}

			std::shared_ptr<bvh_node> Build(bvh_object_list& Objects, std::shared_ptr<bvh_node> Parent, const unsigned int Start, const unsigned int End)
			{
				//Create root the first time we got in the function
				if (!Parent)
				{
					Tree.push_back(std::make_unique<bvh_node>(bvh_node::ROOT, ComputeNodeAABB(Objects, 0, (int)Objects.size())));
					Parent = Tree.back();
					const unsigned int SplitIndex = Start + (End - Start) / 2;
					//Recursively build left child
					Parent->Left = Build(Objects, Parent, Start, SplitIndex);
					//Recursively build right child
					Parent->Right = Build(Objects, Parent, SplitIndex, End);
					return (nullptr);
				}
				//return nullptr if we have no objects
				if (!(Start - End))
				{
					return (nullptr);
				}
				//If we have one object, we put it in a leaf and early exit the function
				if (End - Start == 1)
				{
					//One object per leaf, two leafs
					Tree.push_back(std::make_unique<bvh_node>(bvh_node::LEAF, Objects[Start]->GetAABB()));
					Tree.back()->Parent = Parent;
					Tree.back()->ObjectID = Start;
					return(Tree.back());
				}
				//Choose an axis for the split and sort shapes according to it
				Math::bounding_box NodeBoundingBox = ComputeNodeAABB(Objects, Start, End);
				switch (NodeBoundingBox.MainAxis())
				{
				case 0:
				{
					//X

					//IMPORTANT(MArc) : Finir de debugguer les Sorts

					std::sort(std::begin(Objects) + Start,
						std::begin(Objects) + End,
						[](const std::shared_ptr<Core::actor>& a, const std::shared_ptr<Core::actor>& b)
					{ return a.get()->GetAABB().Center.x > b.get()->GetAABB().Center.x; }
					);
				}break;

				case 1:
				{
					//Y
					std::sort(std::begin(Objects) + Start,
						std::begin(Objects) + End,
						[](const std::shared_ptr<Core::actor>& a, const std::shared_ptr<Core::actor>& b)
					{ return a.get()->GetAABB().Center.y > b.get()->GetAABB().Center.y; }
					);
				}break;

				case 2:
				{
					//Z
					std::sort(std::begin(Objects) + Start,
						std::begin(Objects) + End,
						[](const std::shared_ptr<Core::actor>& a, const std::shared_ptr<Core::actor>& b)
					{ return a.get()->GetAABB().Center.y > b.get()->GetAABB().Center.y; }
					);
				}break;
				}

				Tree.push_back(std::make_unique<bvh_node>(bvh_node::INTERNAL, NodeBoundingBox));
				auto CurrentNode = Tree.back();
				CurrentNode->Parent = Parent;
				//Split in middle, because we don't really care about doing a good job here
				const unsigned int SplitIndex = Start + (End - Start) / 2;
				//Recursively build left child
				CurrentNode->Left = Build(Objects, CurrentNode, Start, SplitIndex);
				//Recursively build right child
				CurrentNode->Right = Build(Objects, CurrentNode, SplitIndex, End);
				return CurrentNode;
			}
		};
	}
}
#endif
