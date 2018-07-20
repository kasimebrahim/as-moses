/** combo_atomese.h ---
 *
 * Copyright (C) 2018 OpenCog Foundation
 *
 * Authors: Kasim Ebrahim <se.kasim.ebrahim@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef MOSES_COMBO_ATOMESE_H
#define MOSES_COMBO_ATOMESE_H

#include <opencog/atoms/base/Handle.h>
#include <moses/comboreduct/combo/vertex.h>
#include <opencog/atoms/base/Link.h>


namespace opencog { namespace combo {

namespace id {
//    Atomese procedure types
enum __attribute__((packed)) procedure_type{
	unknown=0,
	predicate,
	schema
};

}

/**
 * Convert a combo_tree to atomese program.
 *
 * @param combo_tree   the combo_tree containing the combo program
 * @return                 the Handle containing the atomese program
 */
Handle combo2atomese(const combo_tree &tr);

/**
 * Convert a atomese to combo_tree program.
 *
 * @param Handle   the Handle containing the atomese program
 * @return                 the combo_tree containing the combo program
 */
combo_tree atomese2combo(const Handle &h);

/**
 * Convert a combo_tree::vertex to atomese program.
 *
 * @param Handle&       handle ref, if the vertex is converted to node it
 *                      will be stored here
 * @param const vertex&     a vertex ref containing the combo_tree vertex
 *                          to be converted to atom
 * @param procedure_type&     containing the type  of the atom i:e predicate, shema
 * @return                 if the vertex is to be converted to an atomese link
 *                          return Link type otherwise return -1
 */
std::pair<Type, Handle> vertex2atomese(const vertex &, id::procedure_type &);

/**
 * Convert a combo_tree to atomese from a head of a combo_tree program.
 *
 * @param combo_tree::iterator   the iterater to the head of a combo_tree
 * @return                 the Handle containing the atomese program
 */
template<typename Iter>
opencog::Handle combo_it2atomese(Iter it,
                                 id::procedure_type &parent_procedure_type)
{

	id::procedure_type procedure_type = parent_procedure_type;
	combo_tree::iterator head = it;
	std::pair <Type, Handle> atomese= vertex2atomese(*head, procedure_type);
	Type link_type = atomese.first;
	Handle handle = atomese.second;

	if (link_type != (unsigned short) -1) {
		HandleSeq handle_seq;
		for (auto sib = head.begin(); sib != head.end(); ++sib) {
			handle_seq.push_back(combo_it2atomese(sib, procedure_type));
		}
		handle = createLink(handle_seq, link_type);
	}
	return handle;
}

/**
 * Convert a combo argument to atomese.
 *
 * @param Handle&       handle ref, stores the atomese converted from combo argument
 * @param const argument&     a argumet ref containing the combo argument to be
 *                              converted to atom
 * @param procedure_type&     ref to parent procidere type containing the type of
 *                              the atom to be created
 *                            i:e predicateNode, shemaNode
 * @return                 return -1 todo:// return void
 */
Handle argument2atomese(const argument &, const id::procedure_type &);

/**
 * Convert a combo builtin to atomese.
 *
 * @param const builtin&       a builtin ref containing the combo builtin to be
 *                              converted to atom
 * @param procedure_type&     set procedure type from the Link to be created
 *                            i:e predicate, shema
 * @return                 return Link type to be created
 */
Type builtin2atomese(const builtin &, id::procedure_type &);

void atomese2stream(const Handle &h, std::stringstream &, std::vector<std::string> &);

void node2stream(const Handle &h, std::stringstream &, std::vector<std::string> &);

void link2stream(const Handle &h, std::stringstream &);

}}  // ~namespaces combo opencog

#endif //MOSES_COMBO_ATOMESE_H
